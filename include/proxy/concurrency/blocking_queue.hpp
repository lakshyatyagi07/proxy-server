#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

using namespace std;

namespace proxy {

template <typename T>
class BlockingQueue {
public:
    explicit BlockingQueue(size_t max_cap = 10000) : max_cap_(max_cap) {}

    void push(T item) {
        unique_lock<mutex> lock(mtx_);
        cv_push_.wait(lock, [this]() { return q_.size() < max_cap_ || shutdown_; });
        if (shutdown_) return;
        q_.push(move(item));
        cv_pop_.notify_one();
    }

    optional<T> pop() {
        unique_lock<mutex> lock(mtx_);
        cv_pop_.wait(lock, [this]() { return !q_.empty() || shutdown_; });
        if (q_.empty() && shutdown_) return nullopt;
        T item = move(q_.front());
        q_.pop();
        cv_push_.notify_one();
        return item;
    }

    void shutdown() {
        unique_lock<mutex> lock(mtx_);
        shutdown_ = true;
        cv_pop_.notify_all();
        cv_push_.notify_all();
    }

    size_t size() const {
        unique_lock<mutex> lock(mtx_);
        return q_.size();
    }

private:
    queue<T> q_;
    size_t max_cap_;
    bool shutdown_{false};
    mutable mutex mtx_;
    condition_variable cv_pop_;
    condition_variable cv_push_;
};

} // namespace proxy
