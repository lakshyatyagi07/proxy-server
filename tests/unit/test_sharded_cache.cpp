#include "proxy/cache/sharded_cache.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_sharded_cache() {
    proxy::ShardedCache cache(4, 1024 * 1024);
    proxy::HttpResponse res;
    res.status = proxy::HttpStatus::OK;
    res.body = "Test Data";
    cache.put("http://localhost/test", res);

    proxy::HttpResponse out;
    assert(cache.get("http://localhost/test", out));
    assert(out.body == "Test Data");
    cout << "ShardedCache test passed!" << endl;
}

// Update commit 79

// Update commit 104

// Update commit 129
