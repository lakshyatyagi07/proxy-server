#include "proxy/proxy_server.hpp"
#include "proxy/net/tcp_client.hpp"
#include "proxy/http/http_serializer.hpp"
#include "proxy/common/logger.hpp"
#include <thread>
#include <algorithm>

using namespace std;

namespace proxy {

ProxyServer::ProxyServer(uint16_t port, size_t threads, size_t cache_mb)
    : port_(port), thread_pool_(threads > 0 ? threads : 64), cache_(16, cache_mb * 1024 * 1024) {}

ProxyServer::~ProxyServer() { stop(); }

void ProxyServer::start() {
    if (running_) return;
    SocketAddress addr("0.0.0.0", port_);
    if (!server_sock_.set_reuse_addr(true).ok() ||
        !server_sock_.bind(addr).ok() ||
        !server_sock_.listen().ok()) {
        LOG_ERROR("Failed to start ProxyServer on port " + to_string(port_));
        return;
    }

    running_ = true;
    LOG_INFO("ProxyServer running on port " + to_string(port_));

    while (running_) {
        SocketAddress client_addr;
        auto accept_res = server_sock_.accept(client_addr);
        if (accept_res.ok()) {
            Socket client_sock = move(accept_res.value());
            thread_pool_.submit([this, sock = move(client_sock)]() mutable {
                handle_client(move(sock));
            });
        } else {
            if (!running_) break;
        }
    }
}

void ProxyServer::stop() {
    if (running_) {
        running_ = false;
        server_sock_.close();
        thread_pool_.stop();
        LOG_INFO("ProxyServer stopped.");
    }
}

void ProxyServer::handle_client(Socket client_sock) {
    char buf[4096];
    ssize_t n = client_sock.recv(buf, sizeof(buf));
    if (n <= 0) return;

    HttpParser parser;
    auto parse_res = parser.parse(string_view(buf, static_cast<size_t>(n)));
    if (!parse_res.ok() || !parser.complete()) {
        HttpResponse err_res{.status = HttpStatus::BAD_REQUEST, .body = "Bad Request"};
        string raw = HttpSerializer::serialize_response(err_res);
        client_sock.send(raw.data(), raw.size());
        return;
    }

    const auto& req = parser.request();
    if (req.method == HttpMethod::GET) {
        handle_get(client_sock, req);
    } else if (req.method == HttpMethod::CONNECT) {
        // Detach thread for CONNECT HTTPS tunnel so ThreadPool worker remains available
        thread([this, sock = move(client_sock), req]() mutable {
            handle_connect(sock, req);
        }).detach();
    } else {
        HttpResponse err_res{.status = HttpStatus::BAD_REQUEST, .body = "Unsupported Method"};
        string raw = HttpSerializer::serialize_response(err_res);
        client_sock.send(raw.data(), raw.size());
    }
}

void ProxyServer::handle_get(Socket& client_sock, const HttpRequest& req) {
    HttpResponse cached_res;
    if (cache_.get(req.uri, cached_res)) {
        LOG_INFO("Cache HIT: " + req.uri);
        client_sock.send(cached_res.body.data(), cached_res.body.size());
        return;
    }

    LOG_INFO("Cache MISS: " + req.uri);
    string host;
    uint16_t port = 80;
    auto it = req.headers.find("Host");
    if (it != req.headers.end()) {
        host = it->second;
        auto pos = host.find(':');
        if (pos != string::npos) {
            port = static_cast<uint16_t>(stoi(host.substr(pos + 1)));
            host = host.substr(0, pos);
        }
    }

    if (host.empty()) return;

    auto upstream_res = TcpClient::connect(host, port, 3);
    if (!upstream_res.ok()) {
        HttpResponse err_res{.status = HttpStatus::BAD_GATEWAY, .body = "Bad Gateway"};
        string raw = HttpSerializer::serialize_response(err_res);
        client_sock.send(raw.data(), raw.size());
        return;
    }

    Socket upstream_sock = move(upstream_res.value());
    string fwd_req = HttpSerializer::serialize_request(req);
    upstream_sock.send(fwd_req.data(), fwd_req.size());

    char buf[8192];
    string resp_bytes;
    ssize_t n;
    size_t content_len = 0;
    bool is_chunked = false;
    bool headers_parsed = false;
    size_t header_end_pos = string::npos;

    while ((n = upstream_sock.recv(buf, sizeof(buf))) > 0) {
        resp_bytes.append(buf, static_cast<size_t>(n));
        client_sock.send(buf, static_cast<size_t>(n));

        if (!headers_parsed) {
            header_end_pos = resp_bytes.find("\r\n\r\n");
            if (header_end_pos != string::npos) {
                headers_parsed = true;
                string headers_part = resp_bytes.substr(0, header_end_pos);
                if (headers_part.find("Transfer-Encoding: chunked") != string::npos ||
                    headers_part.find("transfer-encoding: chunked") != string::npos) {
                    is_chunked = true;
                }
                auto cl_pos = headers_part.find("Content-Length:");
                if (cl_pos == string::npos) {
                    cl_pos = headers_part.find("content-length:");
                }
                if (cl_pos != string::npos) {
                    auto cl_end = headers_part.find("\r\n", cl_pos);
                    string cl_val = headers_part.substr(cl_pos + 15, cl_end - (cl_pos + 15));
                    try {
                        content_len = stoul(cl_val);
                    } catch (...) {
                        content_len = 0;
                    }
                }
            }
        }

        if (headers_parsed) {
            if (is_chunked) {
                if (resp_bytes.rfind("\r\n0\r\n\r\n") != string::npos ||
                    resp_bytes.rfind("0\r\n\r\n") != string::npos) {
                    break;
                }
            } else if (content_len > 0) {
                size_t body_bytes_received = resp_bytes.size() - (header_end_pos + 4);
                if (body_bytes_received >= content_len) {
                    break;
                }
            }
        }
    }

    if (!resp_bytes.empty()) {
        HttpResponse response{.status = HttpStatus::OK, .body = resp_bytes};
        cache_.put(req.uri, response);
        LOG_INFO("Cache STORED: " + req.uri);
    }
}

void ProxyServer::handle_connect(Socket& client_sock, const HttpRequest& req) {
    string host = req.uri;
    uint16_t port = 443;
    auto pos = host.find(':');
    if (pos != string::npos) {
        port = static_cast<uint16_t>(stoi(host.substr(pos + 1)));
        host = host.substr(0, pos);
    }

    auto upstream_res = TcpClient::connect(host, port, 5);
    if (!upstream_res.ok()) {
        HttpResponse err_res{.status = HttpStatus::BAD_GATEWAY, .body = "Bad Gateway"};
        string raw = HttpSerializer::serialize_response(err_res);
        client_sock.send(raw.data(), raw.size());
        return;
    }

    Socket upstream_sock = move(upstream_res.value());
    string ok_reply = "HTTP/1.1 200 Connection Established\r\n\r\n";
    client_sock.send(ok_reply.data(), ok_reply.size());

    thread t1([c = client_sock.fd(), u = upstream_sock.fd()]() {
        char b[8192];
        ssize_t n;
        while ((n = ::recv(c, b, sizeof(b), 0)) > 0) {
            if (::send(u, b, static_cast<size_t>(n), 0) <= 0) break;
        }
    });

    char b[8192];
    ssize_t n;
    while ((n = upstream_sock.recv(b, sizeof(b))) > 0) {
        if (client_sock.send(b, static_cast<size_t>(n)) <= 0) break;
    }

    if (t1.joinable()) t1.join();
}

} // namespace proxy
