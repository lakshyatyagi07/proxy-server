#pragma once

#include "proxy/common/types.hpp"
#include "proxy/common/result.hpp"
#include <sys/epoll.h>
#include <vector>
#include <functional>

using namespace std;

namespace proxy {

class EpollEventLoop {
public:
    EpollEventLoop();
    ~EpollEventLoop();

    Result<bool> add_fd(SocketFd fd, uint32_t events);
    Result<bool> modify_fd(SocketFd fd, uint32_t events);
    Result<bool> remove_fd(SocketFd fd);

    using EventCallback = function<void(SocketFd fd, uint32_t events)>;
    int wait(int timeout_ms, const EventCallback& cb);

private:
    int epoll_fd_{-1};
    vector<epoll_event> events_;
};

} // namespace proxy
