#include "proxy/net/socket_address.hpp"
#include <arpa/inet.h>
#include <cstring>

using namespace std;

namespace proxy {

SocketAddress::SocketAddress(const string& ip, uint16_t port) {
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
}

SocketAddress::SocketAddress(const sockaddr_in& addr) : addr_(addr) {}

string SocketAddress::ip() const {
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    return string(buf);
}

uint16_t SocketAddress::port() const {
    return ntohs(addr_.sin_port);
}

const sockaddr* SocketAddress::sockaddr_ptr() const {
    return reinterpret_cast<const sockaddr*>(&addr_);
}

socklen_t SocketAddress::sockaddr_len() const {
    return sizeof(addr_);
}

} // namespace proxy

// Update commit 58

// Update commit 83

// Update commit 108

// Update commit 133
