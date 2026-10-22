#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace proxy {

using SocketFd = int;
constexpr SocketFd INVALID_FD = -1;
constexpr size_t BUFFER_SIZE = 8192;

} // namespace proxy
