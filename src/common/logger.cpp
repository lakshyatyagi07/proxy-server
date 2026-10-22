#include "proxy/common/logger.hpp"
#include <chrono>
#include <iomanip>

using namespace std;

namespace proxy {

string level_to_string(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARN: return "WARN";
        case LogLevel::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

Logger& Logger::get() {
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel lvl, const string& msg) {
    lock_guard<mutex> lock(mtx_);
    if (lvl < level_) return;
    cout << "[" << level_to_string(lvl) << "] " << msg << endl;
}

} // namespace proxy

// Update commit 56

// Update commit 81

// Update commit 106

// Update commit 131
