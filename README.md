# Multithreaded C++20 Web Proxy Server

An enterprise-grade, high-performance, multithreaded HTTP/HTTPS forward proxy server built in **C++20** featuring a POSIX Sockets engine, `epoll` event loop, custom thread pool, zero-copy HTTP/1.1 parser, and a thread-safe $O(1)$ LRU cache.

---

## 🌟 Key Features

- **Modern C++20 Standard**: Built with C++20 (`-std=c++20`), RAII abstractions, `std::string_view`, `std::unique_lock`, `std::shared_mutex`, `std::variant`, and designated initializers.
- **POSIX Sockets Network Engine**: Clean low-level abstractions for IPv4 `SocketAddress` (`inet_pton`/`inet_ntop`), RAII `Socket` handling, non-blocking I/O (`O_NONBLOCK`), `SO_REUSEADDR`, `SO_REUSEPORT`, and socket timeout protection.
- **High-Concurrency OS ThreadPool**: Bounded `BlockingQueue` and worker `ThreadPool` dynamically handling concurrent client workloads.
- **Zero-Copy HTTP/1.1 Parser**: State-machine HTTP/1.1 request parser supporting `Content-Length` framing and `Transfer-Encoding: chunked` response stream termination.
- **$O(1)$ LRU & Sharded Cache**: Lock-optimized `ShardedCache` across 16 shards using `std::shared_mutex` for concurrent read hits and thread-safe writes.
- **HTTPS Tunneling**: Transparent `CONNECT` method TCP byte-relaying with detached thread management to prevent worker pool starvation.

---

## 🚀 Getting Started

### Prerequisites
- **C++ Compiler**: GCC 10+ or Clang 11+ with C++20 support.
- **Build Tools**: CMake 3.16+ and Make (`build-essential`).
- **Containerization**: Docker (optional).

---

## 📦 Option A: Native Local Build & Test

### 1. Clone the Repository
```bash
git clone https://github.com/lakshyatyagi07/proxy-server.git
cd proxy-server
```

### 2. Build the Server & Test Suite
```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### 3. Run Unit Tests (11 Test Suites)
```bash
./bin/proxy_tests
```

### 4. Run the Proxy Server
```bash
./bin/proxy_server --port 8080 --threads 64 --cache-mb 64
```

---

## 🐳 Option B: Docker Containerization

### 1. Build Docker Image Locally
```bash
docker build -t lakshyatyagi07/proxy-server:latest .
```

### 2. Run Proxy Server Container
```bash
docker run -d --name proxy_server -p 8080:8080 lakshyatyagi07/proxy-server:latest
```

### 3. Push to Docker Hub
```bash
docker login -u lakshyatyagi07
docker push lakshyatyagi07/proxy-server:latest
```

---

## 🧪 Testing with `curl` & Chromium Browser

### 1. Testing with `curl`
```bash
# HTTP GET Request (Cache MISS on 1st run, Cache HIT on 2nd run)
curl -v -x http://127.0.0.1:8080 http://example.com

# HTTPS CONNECT Tunneling Request
curl -v -x http://127.0.0.1:8080 https://www.google.com
```

### 2. Testing with Chromium Browser
Launch Chromium configured to route all traffic through the proxy:
```bash
chromium --proxy-server="http://127.0.0.1:8080" --user-data-dir="/tmp/chrome_proxy_test"
```
Navigate to `http://example.com` or `https://www.google.com` to observe live real-time caching and HTTPS tunneling logs!
