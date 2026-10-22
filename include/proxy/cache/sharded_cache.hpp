#pragma once

#include "proxy/cache/lru_cache.hpp"
#include "proxy/http/http_types.hpp"
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <memory>

using namespace std;

namespace proxy {

class ShardedCache {
public:
    explicit ShardedCache(size_t num_shards = 16, size_t total_max_bytes = 64 * 1024 * 1024);

    bool get(const string& key, HttpResponse& res);
    void put(const string& key, const HttpResponse& res);

private:
    struct Shard {
        mutable shared_mutex mtx;
        LRUCache<string, HttpResponse> cache;
        explicit Shard(size_t bytes) : cache(bytes) {}
    };

    size_t get_shard_idx(const string& key) const;
    vector<unique_ptr<Shard>> shards_;
};

} // namespace proxy
