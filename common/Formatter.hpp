#pragma once
#include <charconv>
#include <memory>

#include "common/ILoggable.hpp"

namespace std {  // for overload matching when converting a formatter argument
                 // to string
std::string to_string(bool b);
std::string to_string(char c);
std::string to_string(const char *str);
std::string to_string(const std::string &str);
std::string to_string(std::string &&str);
std::string to_string(const rgl::ILoggable &loggable);
std::string to_string(rgl::ILoggable &&loggable);
std::string to_string(std::shared_ptr<rgl::ILoggable> loggable);
}  // namespace std

namespace rgl {
template <typename T>
std::string padLeft(T val, size_t count, char padding = '0') {
  const auto valStr = std::to_string(val);
  if (valStr.size() >= count) {
    return valStr;
  }

  std::string ret(count - valStr.size(), padding);
  return ret + valStr;
}

template <typename... _TArgs>
class Formatter : public ILoggable {
 public:
  Formatter(const std::string &pattern, const _TArgs &... args) {
    m_formatted = "";
    m_formatted.reserve(pattern.size());

    size_t argIdx = 0;
    for (size_t i = 0; i < pattern.size(); i++) {
      if (pattern[i] != '{') {  // if not a formatter
        m_formatted += pattern[i];
        continue;
      }

      size_t endOffset = 1;  // find }
      for (; i + endOffset < pattern.size(); endOffset++) {
        if (pattern[i + endOffset] == '}') break;
      }

      if (i + endOffset >= pattern.size()) {  // if didn't find }
        m_formatted += pattern[i];
        continue;
      }

      if (endOffset == 1) {  // if there is no index in the {}
        m_formatted += getArg(argIdx++, args...);
        i++;
        continue;
      }

      size_t index = 0;
      const char *start = pattern.c_str() + i;
      if (auto [ptr, ec] = std::from_chars(start + 1, start + endOffset, index);
          ec != std::errc() ||
          ptr != start + endOffset) {  // if failed to parse an index
        m_formatted += pattern[i];
        continue;
      }

      m_formatted += getArg(index, args...);
      i += endOffset;
    }
  }

  template <typename _TSep, typename _TFirst, typename... _TRest>
  static std::string join(_TSep sep, _TFirst first, _TRest... args) {
    return std::to_string(first) + std::to_string(sep) + join(sep, args...);
  }
  template <typename _TSep, typename _TArg>
  static std::string join(_TSep sep, _TArg arg) {
    return std::to_string(arg);
  }

  template <typename _TSep, typename _TContainer>
  static std::string joinContainer(const _TSep &sep,
                                   const _TContainer &container) {
    return joinIter(sep, container.cbegin(), container.cend());
  }
  template <typename _TSep, typename _TIter>
  static std::string joinIter(const _TSep &sep, _TIter cbegin, _TIter cend) {
    if (cbegin + 1 == cend) {
      return *cbegin;
    }

    std::string ret = "";
    for (; cbegin + 1 != cend; cbegin++) {
      ret += std::to_string(*cbegin) + std::to_string(sep);
    }
    return ret + std::to_string(*cbegin);
  }

  template <typename _TSep, typename _TContainer, typename _TCallback>
  static std::string joinContainer(const _TSep &sep,
                                   const _TContainer &container,
                                   const _TCallback &callback) {
    return joinIter(sep, container.cbegin(), container.cend(), callback);
  }
  template <typename _TSep, typename _TIter, typename _TCallback>
  static std::string joinIter(const _TSep &sep, _TIter cbegin, _TIter cend,
                                   const _TCallback &callback) {
    if (cbegin + 1 == cend) {
      return callback(*cbegin);
    }

    std::string ret = "";
    for (; cbegin + 1 != cend; cbegin++) {
      ret += std::to_string(callback(*cbegin)) + std::to_string(sep);
    }
    return ret + std::to_string(callback(*cbegin));
  }

  std::string toString() const override { return m_formatted; }

 private:
  std::string m_formatted;

  template <typename _T>
  std::string getArg(size_t index, const _T &arg) {
    return std::to_string(arg);
  }
  template <typename _T, typename... _TRest>
  std::string getArg(size_t index, const _T &arg, const _TRest &... args) {
    if (index == 0) return std::to_string(arg);
    return getArg(index - 1, args...);
  }
};
}  // namespace rgl
