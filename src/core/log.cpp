#include "terra/core/log.h"

#include <cstdio>
#include <cstdarg>

namespace terra {

LogLevel Log::s_level = LogLevel::Info;

void Log::setLevel(LogLevel level) { s_level = level; }
LogLevel Log::level() { return s_level; }

const char* Log::levelName(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info:  return "INFO";
        case LogLevel::Warn:  return "WARN";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Fatal: return "FATAL";
    }
    return "UNKNOWN";
}

template <typename... Args>
void Log::log(LogLevel lvl, const char* fmt, Args... args) {
    if (lvl < s_level) return;
    std::fprintf(stderr, "[%s] ", levelName(lvl));
    // Minimal vararg formatting kept simple for C++14 compatibility.
    // A proper formatter (fmtlib) can be added in a later milestone.
    std::initializer_list<int> unused = { (void(args), 0)... };
    (void)unused;
    std::fputs(fmt, stderr);
    std::fputc('\n', stderr);
}

} // namespace terra
