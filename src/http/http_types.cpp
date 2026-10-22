#include "proxy/http/http_types.hpp"

using namespace std;

namespace proxy {

HttpMethod string_to_method(const string& m) {
    if (m == "GET") return HttpMethod::GET;
    if (m == "POST") return HttpMethod::POST;
    if (m == "PUT") return HttpMethod::PUT;
    if (m == "DELETE") return HttpMethod::DELETE;
    if (m == "CONNECT") return HttpMethod::CONNECT;
    if (m == "HEAD") return HttpMethod::HEAD;
    if (m == "OPTIONS") return HttpMethod::OPTIONS;
    return HttpMethod::UNKNOWN;
}

string method_to_string(HttpMethod m) {
    switch (m) {
        case HttpMethod::GET: return "GET";
        case HttpMethod::POST: return "POST";
        case HttpMethod::PUT: return "PUT";
        case HttpMethod::DELETE: return "DELETE";
        case HttpMethod::CONNECT: return "CONNECT";
        case HttpMethod::HEAD: return "HEAD";
        case HttpMethod::OPTIONS: return "OPTIONS";
        default: return "UNKNOWN";
    }
}

string status_to_reason(HttpStatus s) {
    switch (s) {
        case HttpStatus::OK: return "OK";
        case HttpStatus::BAD_REQUEST: return "Bad Request";
        case HttpStatus::NOT_FOUND: return "Not Found";
        case HttpStatus::BAD_GATEWAY: return "Bad Gateway";
        case HttpStatus::GATEWAY_TIMEOUT: return "Gateway Timeout";
        default: return "Unknown Status";
    }
}

} // namespace proxy

// Update commit 63

// Update commit 88

// Update commit 113

// Update commit 138
