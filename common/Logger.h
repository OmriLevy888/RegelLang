#pragma once
#include <iostream>
#include <ctime>

#include "ILoggable.h"

namespace rgl {
enum class LogLevel : uint8_t {
    debug,
    info,
    warning,
    error,
    critical,
    none
};

class Logger {
private:
    static LogLevel s_logLevel;

    static bool s_prefixType;
    static bool s_prefixDate;

    static bool doPrefixes(LogLevel level, const std::string& type);
    static void log(const std::string& loggable, LogLevel level, const std::string& type);
    static void log(std::string&& loggable, LogLevel level, const std::string& type);

public:
    static inline void setLogLevel(LogLevel level) { s_logLevel = level; }
    static inline LogLevel getLogLevel() { return s_logLevel; }
    static inline void setPrefixType(bool doPrefix) { s_prefixType = doPrefix; }
    static inline bool getPrefixType() { return s_prefixType; }
    static inline void setPrefixDate(bool doPrefix) { s_prefixDate = doPrefix; }
    static inline bool getPrefixDate() { return s_prefixDate; }

    static void init() {
        s_logLevel = LogLevel::info;
        s_prefixType = true;
        s_prefixDate = false;
    }

    static void debug(const ILoggable& loggable);
    static void info(const ILoggable& loggable);
    static void warning(const ILoggable& loggable);
    static void error(const ILoggable& loggable);
    static void critical(const ILoggable& loggable);

    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    static void critical(const std::string& message);

    static void debug(std::string&& message);
    static void info(std::string&& message);
    static void warning(std::string&& message);
    static void error(std::string&& message);
    static void critical(std::string&& message);
};
}  // namespace rgl
