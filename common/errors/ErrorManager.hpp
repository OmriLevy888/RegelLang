#pragma once
#include "common/Formatter.hpp"
#include "common/ILoggable.hpp"
#include <iostream>
#include <string>

namespace rgl {
class ErrorManager {
public:
  template <typename _T, typename... _TArgs>
  static void logError(const _T &first, const _TArgs &...rest) {
    std::cerr << "[!] " << toString(first) << ' ';
    logErrorRest(rest...);
  }
  template <typename _T> static void logError(const _T &error) {
    std::cerr << "[!] " << toString(error) << std::endl;
  }

  template <typename _T, typename... _TArgs>
  static void logWarning(const _T &first, const _TArgs &...rest) {
    std::cerr << "[*] " << toString(first) << ' ';
    logWarningRest(rest...);
  }
  template <typename _T> static void logWarning(const _T &warning) {
    std::cerr << "[*] " << toString(warning) << std::endl;
  }

  template <typename... _TArgs>
  static void logErrorFmt(const std::string &pattern, const _TArgs &...rest) {
    std::cerr << "[!] " << Formatter<_TArgs...>(pattern, rest...).toString()
              << std::endl;
  }

  template <typename... _TArgs>
  static void logWarningFmt(const std::string &pattern, const _TArgs &...rest) {
    std::cerr << "[*] " << Formatter<_TArgs...>(pattern, rest...).toString()
              << std::endl;
  }

private:
  template <typename _T, typename... _TArgs>
  static void logErrorRest(const _T &first, const _TArgs &...rest) {
    std::cerr << toString(first) << ' ';
    logErrorRest(rest...);
  }
  template <typename _T> static void logErrorRest(const _T &error) {
    std::cerr << toString(error) << std::endl;
  }

  template <typename _T, typename... _TArgs>
  static void logWarningRest(const _T &first, const _TArgs &...rest) {
    std::cerr << toString(first) << ' ';
    logWarningRest(rest...);
  }
  template <typename _T> static void logWarningRest(const _T &warning) {
    std::cerr << toString(warning) << std::endl;
  }

  template <typename T> static std::string toString(const T &value) {
    return std::to_string(value);
  }
  static std::string toString(const std::string &str) { return str; }
  static std::string toString(const ILoggable &loggable) {
    return loggable.toString();
  }
};
}; // namespace rgl
