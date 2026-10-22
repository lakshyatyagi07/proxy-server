#include "proxy/http/http_parser.hpp"

using namespace std;

namespace proxy {

void HttpParser::reset() {
    state_ = ParserState::READING_REQUEST_LINE;
    buffer_.clear();
    req_ = HttpRequest();
    content_length_ = 0;
}

Result<bool> HttpParser::parse(string_view data) {
    buffer_.append(data.data(), data.size());
    string_view view(buffer_);

    while (state_ != ParserState::COMPLETE && state_ != ParserState::ERROR) {
        if (state_ == ParserState::READING_REQUEST_LINE) {
            auto pos = view.find("\r\n");
            if (pos == string_view::npos) break;
            string_view line = view.substr(0, pos);
            if (!parse_request_line(line)) {
                state_ = ParserState::ERROR;
                return Error{-1, "Malformed request line"};
            }
            view.remove_prefix(pos + 2);
            state_ = ParserState::READING_HEADERS;
        } else if (state_ == ParserState::READING_HEADERS) {
            auto pos = view.find("\r\n");
            if (pos == string_view::npos) break;
            if (pos == 0) {
                view.remove_prefix(2);
                auto it = req_.headers.find("Content-Length");
                if (it != req_.headers.end()) {
                    try {
                        content_length_ = stoul(it->second);
                    } catch (...) {
                        content_length_ = 0;
                    }
                    state_ = (content_length_ > 0) ? ParserState::READING_BODY : ParserState::COMPLETE;
                } else {
                    state_ = ParserState::COMPLETE;
                }
            } else {
                string_view line = view.substr(0, pos);
                if (!parse_header_line(line)) {
                    state_ = ParserState::ERROR;
                    return Error{-1, "Malformed header line"};
                }
                view.remove_prefix(pos + 2);
            }
        } else if (state_ == ParserState::READING_BODY) {
            if (view.size() >= content_length_) {
                req_.body = string(view.substr(0, content_length_));
                view.remove_prefix(content_length_);
                state_ = ParserState::COMPLETE;
            } else {
                break;
            }
        }
    }
    buffer_ = string(view);
    return complete();
}

bool HttpParser::parse_request_line(string_view line) {
    while (!line.empty() && (line.back() == '\r' || line.back() == ' ')) line.remove_suffix(1);
    auto p1 = line.find(' ');
    if (p1 == string_view::npos) return false;
    auto p2 = line.find(' ', p1 + 1);
    if (p2 == string_view::npos) return false;

    string m(line.substr(0, p1));
    req_.method = string_to_method(m);
    req_.uri = string(line.substr(p1 + 1, p2 - p1 - 1));
    req_.version = string(line.substr(p2 + 1));
    return true;
}

bool HttpParser::parse_header_line(string_view line) {
    while (!line.empty() && (line.back() == '\r' || line.back() == ' ')) line.remove_suffix(1);
    auto pos = line.find(':');
    if (pos == string_view::npos) return false;
    string k(line.substr(0, pos));
    string_view v = line.substr(pos + 1);
    while (!v.empty() && (v.front() == ' ' || v.front() == '\t')) v.remove_prefix(1);
    while (!v.empty() && (v.back() == ' ' || v.back() == '\r' || v.back() == '\t')) v.remove_suffix(1);
    req_.headers[k] = string(v);
    return true;
}

} // namespace proxy
