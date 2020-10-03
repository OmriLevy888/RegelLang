#pragma once
#include "common/Core.h"

namespace rgl {
class Token : public ILoggable {
public:
    Token() : Token(true) {}
    Token(bool isEOF)
    : m_isEOF(isEOF)
    {}

    inline bool isEOF() const { return m_isEOF; }

    std::string toString() const override {
        return "Token<>";
    }

private:
    bool m_isEOF;
};
}  // namespace rgl