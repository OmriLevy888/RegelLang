#pragma once
#include "common/Formatter.hpp"
#include "common/ILoggable.hpp"
#include "common/errors/ErrorObject.hpp"
#include <iostream>
#include <queue>
#include <string>

namespace rgl {
enum class ErrorTypes : size_t {
  E_UNKNOWN_ERROR,

  E_NOT_YET_IMPLEMENTED,

  E_AMBIGIOUS_PROJECT_FILE,
  E_NO_PROJECT_FILE_SPECIFIED,
  E_NO_SUCH_PROJECT_FILE,
  E_NO_BUILD_DEFAULT_NOT_A_STRING,
  E_NO_BUILD_TARGET_AND_NO_DEFAULT,
  E_BUILD_TARGET_DOES_NOT_EXIST,
  E_BUILD_TARGET_IS_NOT_AN_OBJECT,
  E_FAILED_TO_PARSE_BUILD_TARGET,
  E_NO_TARGET_FILES_IN_BUILD_TARGET,

  E_BAD_TOKEN,

  E_MISSING_RHS,

};

enum class WarningTypes : size_t {
  W_MISSING_OUTPUT_FILE,
  W_NON_STRING_NAME_IN_BUILD_TARGET,
};

class ErrorManager {
public:
  static ErrorTypes getErrorType();
  static WarningTypes getWarningType();

  template <typename _T, typename... _TArgs>
  static void logError(ErrorTypes error, const _T &first,
                       const _TArgs &...rest) {
    s_errors.push(error);
    logErrorImpl(first, rest...);
  }
  static void logError(ErrorTypes error, const ErrorObject &obj) {
    s_errors.push(error);
    logErrorImpl(obj);
  }

  template <typename _T, typename... _TArgs>
  static void logWarning(WarningTypes warning, const _T &first,
                         const _TArgs &...rest) {
    s_warnings.push(warning);
    logWarningImpl(first, rest...);
  }

  template <typename... _TArgs>
  static void logErrorFmt(ErrorTypes error, const std::string &pattern,
                          const _TArgs &...rest) {
    s_errors.push(error);
    std::cerr << "[!] " << Formatter<_TArgs...>(pattern, rest...).toString()
              << std::endl;
  }

  template <typename... _TArgs>
  static void logWarningFmt(WarningTypes warning, const std::string &pattern,
                            const _TArgs &...rest) {
    s_warnings.push(warning);
    std::cerr << "[*] " << Formatter<_TArgs...>(pattern, rest...).toString()
              << std::endl;
  }

private:
  static std::queue<ErrorTypes> s_errors;
  static std::queue<WarningTypes> s_warnings;

  template <typename _T, typename... _TArgs>
  static void logErrorImpl(const _T &first, const _TArgs &...rest) {
    std::cerr << "[!] " << toString(first) << ' ';
    logErrorImplRest(rest...);
  }
  template <typename _T> static void logErrorImpl(const _T &error) {
    std::cerr << "[!] " << toString(error) << std::endl;
  }

  template <typename _T, typename... _TArgs>
  static void logWarningImpl(const _T &first, const _TArgs &...rest) {
    std::cerr << "[*] " << toString(first) << ' ';
    logWarningImplRest(rest...);
  }
  template <typename _T> static void logWarningImpl(const _T &warning) {
    std::cerr << "[*] " << toString(warning) << std::endl;
  }

  template <typename _T, typename... _TArgs>
  static void logErrorImplRest(const _T &first, const _TArgs &...rest) {
    std::cerr << toString(first) << ' ';
    logErrorImplRest(rest...);
  }
  template <typename _T> static void logErrorImplRest(const _T &error) {
    std::cerr << toString(error) << std::endl;
  }

  template <typename _T, typename... _TArgs>
  static void logWarningImplRest(const _T &first, const _TArgs &...rest) {
    std::cerr << toString(first) << ' ';
    logWarningImplRest(rest...);
  }
  template <typename _T> static void logWarningImplRest(const _T &warning) {
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
