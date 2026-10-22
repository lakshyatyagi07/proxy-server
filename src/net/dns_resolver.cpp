#include "proxy/net/dns_resolver.hpp"
#include <netdb.h>
#include <cstring>

using namespace std;

namespace proxy {

Result<SocketAddress> DnsResolver::resolve(const string& hostname, uint16_t port) {
    addrinfo hints{}, *res = nullptr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(hostname.c_str(), to_string(port).c_str(), &hints, &res);
    if (err != 0 || !res) {
        return Error{-1, "DNS resolution failed"};
    }
    sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(res->ai_addr);
    SocketAddress addr(*addr_in);
    freeaddrinfo(res);
    return addr;
}

} // namespace proxy

// Update commit 60

// Update commit 85

// Update commit 110

// Update commit 135
