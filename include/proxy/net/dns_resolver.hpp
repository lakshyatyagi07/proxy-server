#pragma once

#include "proxy/net/socket_address.hpp"
#include "proxy/common/result.hpp"
#include <string>

using namespace std;

namespace proxy {

class DnsResolver {
public:
    static Result<SocketAddress> resolve(const string& hostname, uint16_t port);
};

} // namespace proxy
