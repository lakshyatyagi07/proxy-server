#include "proxy/net/tcp_client.hpp"
#include "proxy/net/dns_resolver.hpp"

using namespace std;

namespace proxy {

Result<Socket> TcpClient::connect(const string& host, uint16_t port, int timeout_sec) {
    auto addr_res = DnsResolver::resolve(host, port);
    if (!addr_res.ok()) return addr_res.error();

    Socket sock;
    if (!sock.valid()) return Error{-1, "socket creation failed"};

    sock.set_timeouts(timeout_sec, timeout_sec);
    auto conn_res = sock.connect(addr_res.value());
    if (!conn_res.ok()) return conn_res.error();

    return sock;
}

} // namespace proxy

// Update commit 61

// Update commit 86

// Update commit 111

// Update commit 136
