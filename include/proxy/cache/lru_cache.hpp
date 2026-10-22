#pragma once

#include <unordered_map>
#include <list>
#include <string>

using namespace std;

namespace proxy {

template <typename Key, typename Value>
class LRUCache {
public:
    explicit LRUCache(size_t max_bytes = 64 * 1024 * 1024) : max_bytes_(max_bytes) {}

    bool get(const Key& key, Value& val) {
        auto it = map_.find(key);
        if (it == map_.end()) return false;
        items_.splice(items_.begin(), items_, it->second);
        val = it->second->val;
        return true;
    }

    void put(const Key& key, Value val, size_t bytes) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            cur_bytes_ -= it->second->bytes;
            items_.splice(items_.begin(), items_, it->second);
            it->second->val = val;
            it->second->bytes = bytes;
            cur_bytes_ += bytes;
            evict();
            return;
        }
        items_.push_front(Node{key, val, bytes});
        map_[key] = items_.begin();
        cur_bytes_ += bytes;
        evict();
    }

    size_t size() const { return map_.size(); }

private:
    struct Node {
        Key key;
        Value val;
        size_t bytes;
    };

    void evict() {
        while (cur_bytes_ > max_bytes_ && !items_.empty()) {
            auto last = items_.end();
            --last;
            cur_bytes_ -= last->bytes;
            map_.erase(last->key);
            items_.pop_back();
        }
    }

    size_t max_bytes_;
    size_t cur_bytes_{0};
    list<Node> items_;
    unordered_map<Key, typename list<Node>::iterator> map_;
};

} // namespace proxy
