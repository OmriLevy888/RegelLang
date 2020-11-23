#pragma once

#include "common/Core.h"
#include "lexer/Token.h"

#include <vector>

namespace rgl {
class SourceLine : public ILoggable {
public:
    std::string m_repr;
    std::vector<Token> m_tokens;

    SourceLine(const std::string &repr)
    : m_repr(repr)
    {}

    std::string pointAt(size_t idx) const {
        if (idx >= m_tokens.size()) {
            return "";
        }

        const std::string_view repr{
            m_repr.c_str() + m_tokens[idx].getReprStartIdx(),
            m_tokens[idx].getReprLen()};
        
        size_t numSpaces = repr.cbegin() - m_repr.c_str();
        size_t numUnderline = repr.length();
        size_t numHandle = numSpaces + (numUnderline / 2);
        std::string spaces(numSpaces, ' ');
        std::string underline(numUnderline, '^');
        std::string handle(numHandle, '_');

        return Formatter("{}\n{}{}\n{}|", 
            m_repr, spaces, underline, handle).toString();
    }

    std::string toString() const override {
        return Formatter("SourceLine<{}>", m_repr).toString();
    }
};
}  // namespace rgl

//func foo(int a, char b, starng c):
//                        ^^^^^^
//__________________________|