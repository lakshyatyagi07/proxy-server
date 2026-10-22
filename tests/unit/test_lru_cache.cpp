#include "proxy/cache/lru_cache.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_lru_cache() {
    proxy::LRUCache<string, string> cache(100);
    cache.put("k1", "v1", 20);
    string val;
    assert(cache.get("k1", val) && val == "v1");
    cout << "LRUCache test passed!" << endl;
}

// Update commit 78

// Update commit 103

// Update commit 128
