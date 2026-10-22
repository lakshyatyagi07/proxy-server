#pragma once

#include "proxy/http/http_types.hpp"
#include <string>

using namespace std;

namespace proxy {

class HttpSerializer {
public:
    static string serialize_response(const HttpResponse& res);
    static string serialize_request(const HttpRequest& req);
};

} // namespace proxy
