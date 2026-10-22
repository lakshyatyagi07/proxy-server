#include "proxy/proxy_server.hpp"
#include "proxy/common/logger.hpp"
#include <csignal>
#include <iostream>
#include <memory>

using namespace std;

static unique_ptr<proxy::ProxyServer> g_server;

void signal_handler(int sig) {
    LOG_INFO("Received signal " + to_string(sig) + ", shutting down proxy server...");
    if (g_server) g_server->stop();
}

int main(int argc, char* argv[]) {
    uint16_t port = 8080;
    size_t threads = 8;
    size_t cache_mb = 64;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--port" && i + 1 < argc) port = static_cast<uint16_t>(stoi(argv[++i]));
        else if (arg == "--threads" && i + 1 < argc) threads = static_cast<size_t>(stoi(argv[++i]));
        else if (arg == "--cache-mb" && i + 1 < argc) cache_mb = static_cast<size_t>(stoi(argv[++i]));
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    g_server = make_unique<proxy::ProxyServer>(port, threads, cache_mb);
    g_server->start();

    return 0;
}

// Update commit 68

// Update commit 93

// Update commit 118
