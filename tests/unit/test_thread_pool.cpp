#include "proxy/concurrency/thread_pool.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_thread_pool() {
    proxy::ThreadPool pool(4);
    auto fut = pool.submit([](int a, int b) { return a + b; }, 12, 30);
    assert(fut.get() == 42);
    cout << "ThreadPool test passed!" << endl;
}

// Update commit 75

// Update commit 100

// Update commit 125
