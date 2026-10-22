#pragma once

#include "proxy/concurrency/blocking_queue.hpp"
#include <vector>
#include <thread>
#include <future>
#include <functional>

using namespace std;

namespace proxy {

class ThreadPool {
public:
    explicit ThreadPool(size_t threads = thread::hardware_concurrency());
    ~ThreadPool();

    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> future<invoke_result_t<F, Args...>> {
        using return_type = invoke_result_t<F, Args...>;
        auto task = make_shared<packaged_task<return_type()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        future<return_type> res = task->get_future();
        queue_.push([task]() { (*task)(); });
        return res;
    }

    void stop();
    size_t worker_count() const { return workers_.size(); }

private:
    vector<thread> workers_;
    BlockingQueue<function<void()>> queue_;
    bool stopped_{false};
};

} // namespace proxy
