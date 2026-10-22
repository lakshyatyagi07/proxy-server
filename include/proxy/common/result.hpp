#pragma once

#include <string>
#include <variant>

using namespace std;

namespace proxy {

struct Error {
    int code{0};
    string msg;
};

template <typename T>
class Result {
public:
    Result(const T& val) : data_(val) {}
    Result(T&& val) : data_(move(val)) {}
    Result(const Error& err) : data_(err) {}

    bool ok() const { return holds_alternative<T>(data_); }
    const T& value() const { return get<T>(data_); }
    T& value() { return get<T>(data_); }
    const Error& error() const { return get<Error>(data_); }

private:
    variant<T, Error> data_;
};

} // namespace proxy
