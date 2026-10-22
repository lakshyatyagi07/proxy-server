#include "proxy/http/http_serializer.hpp"
#include <sstream>

using namespace std;

namespace proxy {

string HttpSerializer::serialize_response(const HttpResponse& res) {
    stringstream ss;
    ss << res.version << " " << static_cast<int>(res.status) << " " << status_to_reason(res.status) << "\r\n";
    HeaderMap headers = res.headers;
    if (headers.find("Content-Length") == headers.end() && !res.body.empty()) {
        headers["Content-Length"] = to_string(res.body.size());
    }
    for (const auto& [k, v] : headers) {
        ss << k << ": " << v << "\r\n";
    }
    ss << "\r\n" << res.body;
    return ss.str();
}

string HttpSerializer::serialize_request(const HttpRequest& req) {
    stringstream ss;
    ss << method_to_string(req.method) << " " << req.uri << " " << req.version << "\r\n";
    for (const auto& [k, v] : req.headers) {
        ss << k << ": " << v << "\r\n";
    }
    ss << "\r\n" << req.body;
    return ss.str();
}

} // namespace proxy

// Update commit 64

// Update commit 89

// Update commit 114

// Update commit 139
