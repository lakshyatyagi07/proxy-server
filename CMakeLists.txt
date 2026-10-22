cmake_minimum_required(VERSION 3.16)
project(MultithreadedProxyServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerFlags)
include(Sanitizers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Threads REQUIRED)

add_library(proxy_common src/common/logger.cpp)

add_library(proxy_net
    src/common/logger.cpp
    src/net/socket_address.cpp
    src/net/socket.cpp
    src/net/epoll_event_loop.cpp
    src/net/dns_resolver.cpp
    src/net/tcp_client.cpp
)

add_library(proxy_concurrency
    src/common/logger.cpp
    src/concurrency/thread_pool.cpp
)
target_link_libraries(proxy_concurrency PRIVATE Threads::Threads)

add_library(proxy_http
    src/common/logger.cpp
    src/http/http_types.cpp
    src/http/http_serializer.cpp
    src/http/http_parser.cpp
)

add_library(proxy_cache
    src/common/logger.cpp
    src/cache/sharded_cache.cpp
)

add_library(proxy_core
    src/proxy_server.cpp
)
target_link_libraries(proxy_core PRIVATE proxy_net proxy_concurrency proxy_http proxy_cache Threads::Threads)

add_executable(proxy_server src/main.cpp)
target_link_libraries(proxy_server PRIVATE proxy_core Threads::Threads)

add_executable(proxy_tests
    tests/test_main.cpp
    tests/unit/test_logger.cpp
    tests/unit/test_socket_address.cpp
    tests/unit/test_socket.cpp
    tests/unit/test_epoll_event_loop.cpp
    tests/unit/test_blocking_queue.cpp
    tests/unit/test_thread_pool.cpp
    tests/unit/test_http_serializer.cpp
    tests/unit/test_http_parser.cpp
    tests/unit/test_lru_cache.cpp
    tests/unit/test_sharded_cache.cpp
    tests/unit/test_dns_resolver.cpp
)
target_link_libraries(proxy_tests PRIVATE proxy_core Threads::Threads)
