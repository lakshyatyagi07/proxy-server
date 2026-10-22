#include "proxy/net/socket.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_socket() {
    proxy::Socket sock;
    assert(sock.valid());
    assert(sock.set_reuse_addr(true).ok());
    cout << "Socket unit test passed!" << endl;
}

// Update commit 71

// Update commit 96

// Update commit 121
