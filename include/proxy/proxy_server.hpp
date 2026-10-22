#pragma once

#include "proxy/net/socket.hpp"
#include "proxy/concurrency/thread_pool.hpp"
#include "proxy/cache/sharded_cache.hpp"
#include "proxy/http/http_parser.hpp"
#include <atomic>

using namespace std;

namespace proxy {

class ProxyServer {
public:
    ProxyServer(uint16_t port, size_t threads, size_t cache_mb);
    ~ProxyServer();

    void start();
    void stop();

private:
    void handle_client(Socket client_sock);
    void handle_get(Socket& client_sock, const HttpRequest& req);
    void handle_connect(Socket& client_sock, const HttpRequest& req);

    uint16_t port_;
    Socket server_sock_;
    ThreadPool thread_pool_;
    ShardedCache cache_;
    atomic<bool> running_{false};
};

} // namespace proxy
