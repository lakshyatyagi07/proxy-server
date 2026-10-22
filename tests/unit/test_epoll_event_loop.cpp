#include "proxy/net/epoll_event_loop.hpp"
#include "proxy/net/socket.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_epoll_event_loop() {
    proxy::EpollEventLoop loop;
    proxy::Socket sock;
    assert(loop.add_fd(sock.fd(), EPOLLIN | EPOLLET).ok());
    cout << "EpollEventLoop test passed!" << endl;
}

// Update commit 73

// Update commit 98

// Update commit 123
