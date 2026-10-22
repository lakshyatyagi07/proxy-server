#include "proxy/net/epoll_event_loop.hpp"
#include <unistd.h>

using namespace std;

namespace proxy {

EpollEventLoop::EpollEventLoop() : epoll_fd_(epoll_create1(EPOLL_CLOEXEC)), events_(1024) {}

EpollEventLoop::~EpollEventLoop() {
    if (epoll_fd_ >= 0) ::close(epoll_fd_);
}

Result<bool> EpollEventLoop::add_fd(SocketFd fd, uint32_t events) {
    epoll_event ev{.events = events, .data = {.fd = fd}};
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) < 0) return Error{-1, "epoll add failed"};
    return true;
}

Result<bool> EpollEventLoop::modify_fd(SocketFd fd, uint32_t events) {
    epoll_event ev{.events = events, .data = {.fd = fd}};
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev) < 0) return Error{-1, "epoll mod failed"};
    return true;
}

Result<bool> EpollEventLoop::remove_fd(SocketFd fd) {
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr) < 0) return Error{-1, "epoll del failed"};
    return true;
}

int EpollEventLoop::wait(int timeout_ms, const EventCallback& cb) {
    int nfds = epoll_wait(epoll_fd_, events_.data(), static_cast<int>(events_.size()), timeout_ms);
    for (int i = 0; i < nfds; ++i) {
        cb(events_[i].data.fd, events_[i].events);
    }
    return nfds;
}

} // namespace proxy

// Update commit 59

// Update commit 84

// Update commit 109

// Update commit 134
