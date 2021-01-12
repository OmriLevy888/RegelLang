#pragma once

#include "common/Formatter.hpp"
#include "common/ILoggable.hpp"
#include "lexer/Token.hpp"

#include <vector>

namespace rgl {

class SourceLine : public ILoggable {
public:
  std::string m_repr;
  std::vector<Token> m_tokens;

  SourceLine(const std::string &repr) : m_repr(repr) {}
  SourceLine(std::string &&repr) : m_repr(std::move(repr)) {}
  SourceLine(std::string &&repr, const std::vector<TokenValuePair> tokens)
      : m_repr(repr) {
    m_tokens.reserve(tokens.size());
    for (const auto &token : tokens) {
      m_tokens.push_back(token.m_token);
    }
  }
  SourceLine(std::string &&repr, const std::vector<TokenValuePair> tokens,
             const size_t lineNo)
      : m_repr(repr) {
    m_tokens.reserve(tokens.size());
    for (const auto &token : tokens) {
      if (lineNo != token.m_token.getLineNo()) {
        continue;
      }
      m_tokens.push_back(token.m_token);
    }
  }

  std::pair<std::string, std::string> pointAt(size_t idx) const {
    if (idx >= m_tokens.size()) {
      return {"", ""};
    }

    const std::string_view repr{m_repr.c_str() +
                                    m_tokens[idx].getReprStartIdx(),
                                m_tokens[idx].getReprLen()};

    size_t numSpaces = repr.cbegin() - m_repr.c_str();
    size_t numUnderline = repr.length();
    size_t numHandle = numSpaces + (numUnderline / 2);
    std::string spaces(numSpaces, ' ');
    std::string underline(numUnderline, '^');
    std::string handle(numHandle, '_');

    return {spaces + underline, handle + "|"};
  }

  std::string toString() const override {
    return Formatter("SourceLine<{}>", m_repr).toString();
  }
};
} // namespace rgl
