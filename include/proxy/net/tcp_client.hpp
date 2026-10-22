#pragma once

#include "proxy/net/socket.hpp"
#include "proxy/common/result.hpp"

using namespace std;

namespace proxy {

class TcpClient {
public:
    static Result<Socket> connect(const string& host, uint16_t port, int timeout_sec = 5);
};

} // namespace proxy
