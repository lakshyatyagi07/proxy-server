#pragma once

#include "proxy/common/types.hpp"
#include "proxy/common/result.hpp"
#include "proxy/net/socket_address.hpp"
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

namespace proxy {

class Socket {
public:
    Socket();
    explicit Socket(SocketFd fd);
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    SocketFd fd() const { return fd_; }
    bool valid() const { return fd_ != INVALID_FD; }
    void close();

    Result<bool> set_reuse_addr(bool enable);
    Result<bool> set_reuse_port(bool enable);
    Result<bool> set_nonblocking(bool enable);
    Result<bool> set_timeouts(int rcv_sec, int snd_sec);

    Result<bool> bind(const SocketAddress& addr);
    Result<bool> listen(int backlog = SOMAXCONN);
    Result<Socket> accept(SocketAddress& client_addr);
    Result<bool> connect(const SocketAddress& addr);

    ssize_t send(const void* buf, size_t len);
    ssize_t recv(void* buf, size_t len);

private:
    SocketFd fd_{INVALID_FD};
};

} // namespace proxy
