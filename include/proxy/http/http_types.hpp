#pragma once

#include <string>
#include <unordered_map>

using namespace std;

namespace proxy {

enum class HttpMethod {
    UNKNOWN,
    GET,
    POST,
    PUT,
    DELETE,
    CONNECT,
    HEAD,
    OPTIONS
};

HttpMethod string_to_method(const string& m);
string method_to_string(HttpMethod m);

enum class HttpStatus {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    BAD_GATEWAY = 502,
    GATEWAY_TIMEOUT = 504
};

string status_to_reason(HttpStatus s);

using HeaderMap = unordered_map<string, string>;

struct HttpRequest {
    HttpMethod method{HttpMethod::UNKNOWN};
    string uri;
    string path;
    string version{"HTTP/1.1"};
    HeaderMap headers;
    string body;
};

struct HttpResponse {
    HttpStatus status{HttpStatus::OK};
    string version{"HTTP/1.1"};
    HeaderMap headers;
    string body;
};

} // namespace proxy
