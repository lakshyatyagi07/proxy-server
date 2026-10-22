#pragma once

#include <string>
#include <mutex>
#include <iostream>

using namespace std;

namespace proxy {

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

string level_to_string(LogLevel lvl);

class Logger {
public:
    static Logger& get();

    void set_level(LogLevel lvl) {
        lock_guard<mutex> lock(mtx_);
        level_ = lvl;
    }

    void log(LogLevel lvl, const string& msg);

private:
    Logger() = default;
    LogLevel level_{LogLevel::INFO};
    mutable mutex mtx_;
};

#define LOG_INFO(msg) proxy::Logger::get().log(proxy::LogLevel::INFO, msg)
#define LOG_ERROR(msg) proxy::Logger::get().log(proxy::LogLevel::ERROR, msg)

} // namespace proxy
