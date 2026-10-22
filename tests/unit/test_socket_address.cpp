#include "proxy/net/socket_address.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_socket_address() {
    proxy::SocketAddress addr("127.0.0.1", 8080);
    assert(addr.ip() == "127.0.0.1");
    assert(addr.port() == 8080);
    cout << "SocketAddress test passed!" << endl;
}

// Update commit 72

// Update commit 97

// Update commit 122
