#include <iostream>

using namespace std;

void test_logger();
void test_socket_address();
void test_socket();
void test_epoll_event_loop();
void test_blocking_queue();
void test_thread_pool();
void test_http_serializer();
void test_http_parser();
void test_lru_cache();
void test_sharded_cache();
void test_dns_resolver();

int main() {
    cout << "Running multithreaded proxy unit tests..." << endl;
    test_logger();
    test_socket_address();
    test_socket();
    test_epoll_event_loop();
    test_blocking_queue();
    test_thread_pool();
    test_http_serializer();
    test_http_parser();
    test_lru_cache();
    test_sharded_cache();
    test_dns_resolver();
    cout << "All unit tests completed successfully!" << endl;
    return 0;
}

// Update commit 69

// Update commit 94

// Update commit 119
