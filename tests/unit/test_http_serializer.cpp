#include "proxy/http/http_serializer.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_http_serializer() {
    proxy::HttpResponse res;
    res.status = proxy::HttpStatus::OK;
    res.body = "Hello World";
    string str = proxy::HttpSerializer::serialize_response(res);
    assert(str.find("200 OK") != string::npos);
    cout << "HttpSerializer test passed!" << endl;
}

// Update commit 76

// Update commit 101

// Update commit 126
