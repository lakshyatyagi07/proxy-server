#include "proxy/concurrency/blocking_queue.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_blocking_queue() {
    proxy::BlockingQueue<int> q(5);
    q.push(42);
    assert(q.size() == 1);
    auto val = q.pop();
    assert(val.has_value() && val.value() == 42);
    cout << "BlockingQueue test passed!" << endl;
}

// Update commit 74

// Update commit 99

// Update commit 124
