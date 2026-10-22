#include "proxy/cache/sharded_cache.hpp"
#include <mutex>
#include <shared_mutex>

using namespace std;

namespace proxy {

ShardedCache::ShardedCache(size_t num_shards, size_t total_max_bytes) {
    if (num_shards == 0) num_shards = 1;
    size_t bytes_per_shard = total_max_bytes / num_shards;
    shards_.reserve(num_shards);
    for (size_t i = 0; i < num_shards; ++i) {
        shards_.push_back(make_unique<Shard>(bytes_per_shard));
    }
}

size_t ShardedCache::get_shard_idx(const string& key) const {
    return hash<string>{}(key) % shards_.size();
}

bool ShardedCache::get(const string& key, HttpResponse& res) {
    size_t idx = get_shard_idx(key);
    shared_lock<shared_mutex> lock(shards_[idx]->mtx);
    return shards_[idx]->cache.get(key, res);
}

void ShardedCache::put(const string& key, const HttpResponse& res) {
    size_t idx = get_shard_idx(key);
    unique_lock<shared_mutex> lock(shards_[idx]->mtx);
    size_t bytes = res.body.size() + 256;
    shards_[idx]->cache.put(key, res, bytes);
}

} // namespace proxy

// Update commit 66

// Update commit 91

// Update commit 116
