#include "proxy/net/socket.hpp"
#include <fcntl.h>
#include <cstring>

using namespace std;

namespace proxy {

Socket::Socket() : fd_(socket(AF_INET, SOCK_STREAM, 0)) {}
Socket::Socket(SocketFd fd) : fd_(fd) {}

Socket::~Socket() { close(); }

Socket::Socket(Socket&& other) noexcept : fd_(other.fd_) {
    other.fd_ = INVALID_FD;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        close();
        fd_ = other.fd_;
        other.fd_ = INVALID_FD;
    }
    return *this;
}

void Socket::close() {
    if (fd_ != INVALID_FD) {
        ::close(fd_);
        fd_ = INVALID_FD;
    }
}

Result<bool> Socket::set_reuse_addr(bool enable) {
    int opt = enable ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    return true;
}

Result<bool> Socket::set_reuse_port(bool enable) {
    int opt = enable ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    return true;
}

Result<bool> Socket::set_nonblocking(bool enable) {
    int flags = fcntl(fd_, F_GETFL, 0);
    flags = enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    fcntl(fd_, F_SETFL, flags);
    return true;
}

Result<bool> Socket::set_timeouts(int rcv_sec, int snd_sec) {
    timeval tv{.tv_sec = rcv_sec, .tv_usec = 0};
    setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    tv.tv_sec = snd_sec;
    setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    return true;
}

Result<bool> Socket::bind(const SocketAddress& addr) {
    if (::bind(fd_, addr.sockaddr_ptr(), addr.sockaddr_len()) < 0) {
        return Error{-1, "bind failed"};
    }
    return true;
}

Result<bool> Socket::listen(int backlog) {
    if (::listen(fd_, backlog) < 0) {
        return Error{-1, "listen failed"};
    }
    return true;
}

Result<Socket> Socket::accept(SocketAddress& client_addr) {
    sockaddr_in in_addr{};
    socklen_t len = sizeof(in_addr);
    SocketFd cfd = ::accept(fd_, reinterpret_cast<sockaddr*>(&in_addr), &len);
    if (cfd < 0) return Error{-1, "accept failed"};
    client_addr = SocketAddress(in_addr);
    return Socket(cfd);
}

Result<bool> Socket::connect(const SocketAddress& addr) {
    if (::connect(fd_, addr.sockaddr_ptr(), addr.sockaddr_len()) < 0) {
        if (errno != EINPROGRESS) return Error{-1, "connect failed"};
    }
    return true;
}

ssize_t Socket::send(const void* buf, size_t len) {
    return ::send(fd_, buf, len, 0);
}

ssize_t Socket::recv(void* buf, size_t len) {
    return ::recv(fd_, buf, len, 0);
}

} // namespace proxy

// Update commit 57

// Update commit 82

// Update commit 107

// Update commit 132
