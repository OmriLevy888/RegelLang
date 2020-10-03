#pragma once
#include "Core.h"
#include "ILoggable.h"

#include <string_view>
#include <charconv>

namespace std {  // for overload matching when converting a formatter argument to stirng
    std::string to_string(const std::string& str);
    std::string to_string(std::string&& str);
    std::string to_string(const rgl::ILoggable& loggable);
    std::string to_string(rgl::ILoggable&& loggable);
}

namespace rgl {
template<typename ...TArgs>
class Formatter : public ILoggable {
public:
    Formatter(std::string&& pattern, const TArgs&... args) {
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
                ec != std::errc() || ptr != start + endOffset) {  // if failed to parse an index
                m_formatted += pattern[i];
                continue;
            }

            m_formatted += getArg(index, args...);
            i += endOffset;
        }
    }

    std::string toString() const override { return m_formatted; }

private:
    std::string m_formatted;

    template<typename _T>
    std::string getArg(size_t index, const _T& arg) {
        return std::to_string(arg);
    }
    template<typename _T, typename ..._TArgs>
    std::string getArg(size_t index, const _T& arg, const _TArgs&... args) {
        if (index == 0) return std::to_string(arg);
        return getArg(index - 1, args...);
    }
};
}  // namespace rgl