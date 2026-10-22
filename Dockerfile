# Multi-stage build for Multithreaded C++20 Proxy Server
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir -p build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

# Final minimal runtime image
FROM ubuntu:22.04 AS runner

WORKDIR /app

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/bin/proxy_server /app/proxy_server
COPY --from=builder /app/build/bin/proxy_tests /app/proxy_tests

EXPOSE 8080

ENTRYPOINT ["/app/proxy_server"]
CMD ["--port", "8080", "--threads", "64", "--cache-mb", "64"]
