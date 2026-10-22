#pragma once

#include "proxy/http/http_types.hpp"
#include "proxy/common/result.hpp"
#include <string_view>
#include <string>

using namespace std;

namespace proxy {

enum class ParserState {
    READING_REQUEST_LINE,
    READING_HEADERS,
    READING_BODY,
    COMPLETE,
    ERROR
};

class HttpParser {
public:
    HttpParser() = default;

    Result<bool> parse(string_view data);
    bool complete() const { return state_ == ParserState::COMPLETE; }
    void reset();

    const HttpRequest& request() const { return req_; }

private:
    ParserState state_{ParserState::READING_REQUEST_LINE};
    string buffer_;
    HttpRequest req_;
    size_t content_length_{0};

    bool parse_request_line(string_view line);
    bool parse_header_line(string_view line);
};

} // namespace proxy
