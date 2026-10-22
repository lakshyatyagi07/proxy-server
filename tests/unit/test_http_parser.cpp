#include "proxy/http/http_parser.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_http_parser() {
    proxy::HttpParser parser;
    string req = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
    auto res = parser.parse(req);
    assert(res.ok() && res.value() == true);
    assert(parser.request().method == proxy::HttpMethod::GET);
    assert(parser.request().uri == "/index.html");
    cout << "HttpParser test passed!" << endl;
}

// Update commit 77

// Update commit 102

// Update commit 127
