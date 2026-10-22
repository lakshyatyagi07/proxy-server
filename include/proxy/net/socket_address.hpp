#pragma once

#include <netinet/in.h>
#include <string>

using namespace std;

namespace proxy {

class SocketAddress {
public:
    SocketAddress() = default;
    SocketAddress(const string& ip, uint16_t port);
    explicit SocketAddress(const sockaddr_in& addr);

    string ip() const;
    uint16_t port() const;

    const sockaddr* sockaddr_ptr() const;
    socklen_t sockaddr_len() const;

private:
    sockaddr_in addr_{};
};

} // namespace proxy
