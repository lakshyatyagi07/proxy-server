#include "proxy/concurrency/thread_pool.hpp"

using namespace std;

namespace proxy {

ThreadPool::ThreadPool(size_t threads) {
    if (threads == 0) threads = 1;
    workers_.reserve(threads);
    for (size_t i = 0; i < threads; ++i) {
        workers_.emplace_back([this]() {
            while (true) {
                auto task = queue_.pop();
                if (!task.has_value()) break;
                (*task)();
            }
        });
    }
}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::stop() {
    if (!stopped_) {
        stopped_ = true;
        queue_.shutdown();
        for (auto& w : workers_) {
            if (w.joinable()) w.join();
        }
    }
}

} // namespace proxy

// Update commit 62

// Update commit 87

// Update commit 112

// Update commit 137
