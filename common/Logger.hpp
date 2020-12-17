#pragma once

#include "common/Core.hpp"

#include <ctime>
#include <iostream>

namespace rgl {
enum class LogLevel : uint8_t { debug, info, warning, error, critical, none };

class Logger {
private:
  static LogLevel s_logLevel;

  static bool s_prefixType;
  static bool s_prefixDate;

  static bool doPrefixes(LogLevel level, const std::string &type);
  static void log(const std::string &loggable, LogLevel level,
                  const std::string &type);
  static void log(std::string &&loggable, LogLevel level,
                  const std::string &type);

  template <typename _TSep, typename _TFirst, typename... _TArgs>
  static std::string join(_TSep sep, _TFirst first, _TArgs... args) {
    return std::to_string(first) + std::to_string(sep) +
           Logger::join(sep, args...);
  }
  template <typename _TSep, typename _TArg>
  static std::string join(_TSep sep, _TArg arg) {
    return std::to_string(arg);
  }

public:
  static inline void setLogLevel(LogLevel level) noexcept {
    s_logLevel = level;
  }
  static inline LogLevel getLogLevel() noexcept { return s_logLevel; }
  static inline void setPrefixType(bool doPrefix) noexcept {
    s_prefixType = doPrefix;
  }
  static inline bool getPrefixType() noexcept { return s_prefixType; }
  static inline void setPrefixDate(bool doPrefix) noexcept {
    s_prefixDate = doPrefix;
  }
  static inline bool getPrefixDate() noexcept { return s_prefixDate; }

  static void init() {
    s_logLevel = LogLevel::info;
    s_prefixType = true;
    s_prefixDate = false;
  }

  template <typename... _TArgs> static void debug(_TArgs... args) {
    log(Logger::join(' ', args...), LogLevel::debug, "Debug");
  }
  template <typename... _TArgs> static void info(_TArgs... args) {
    log(Logger::join(' ', args...), LogLevel::info, "Info");
  }
  template <typename... _TArgs> static void warning(_TArgs... args) {
    log(Logger::join(' ', args...), LogLevel::warning, "Warning");
  }
  template <typename... _TArgs> static void error(_TArgs... args) {
    log(Logger::join(' ', args...), LogLevel::error, "Error");
  }
  template <typename... _TArgs> static void critical(_TArgs... args) {
    log(Logger::join(' ', args...), LogLevel::critical, "Critical");
  }
};
} // namespace rgl
