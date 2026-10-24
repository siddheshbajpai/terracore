#pragma once

#include <string>
#include <cstdio>

namespace terra {

// Minimal, dependency-free logging. Thread-unsafe by design for the MVP;
// synchronization can be added in a later milestone.
enum class LogLevel {
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

class Log {
public:
    static void setLevel(LogLevel level);
    static LogLevel level();

    template <typename... Args>
    static void log(LogLevel lvl, const char* fmt, Args... args);

    static void debug(const char* msg)  { log(LogLevel::Debug,  "%s", msg); }
    static void info (const char* msg)  { log(LogLevel::Info,   "%s", msg); }
    static void warn (const char* msg)  { log(LogLevel::Warn,   "%s", msg); }
    static void error(const char* msg)  { log(LogLevel::Error,  "%s", msg); }
    static void fatal(const char* msg)  { log(LogLevel::Fatal,  "%s", msg); }

private:
    static LogLevel s_level;
    static const char* levelName(LogLevel lvl);
};

#define TERRA_LOG_DEBUG(msg) terra::Log::debug(msg)
#define TERRA_LOG_INFO(msg)  terra::Log::info(msg)
#define TERRA_LOG_WARN(msg)  terra::Log::warn(msg)
#define TERRA_LOG_ERROR(msg) terra::Log::error(msg)
#define TERRA_LOG_FATAL(msg) terra::Log::fatal(msg)

} // namespace terra
