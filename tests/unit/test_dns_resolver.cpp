#include "proxy/net/dns_resolver.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_dns_resolver() {
    auto res = proxy::DnsResolver::resolve("localhost", 80);
    assert(res.ok());
    cout << "DnsResolver test passed!" << endl;
}

// Update commit 80

// Update commit 105

// Update commit 130
