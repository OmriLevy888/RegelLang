#pragma once
#include "common/Core.hpp"
#include "lexer/Token.hpp"
#include <variant>

namespace rgl {
using TokenValue =
    std::variant<bool, char, int64_t, uint64_t, float, double, std::string>;

class TokenValuePair {
public:
  Token m_token;
  std::optional<TokenValue> m_value;

  TokenValuePair(Token &&token) : m_token(token), m_value(std::nullopt) {}
  TokenValuePair(Token &&token, std::optional<TokenValue> &&value)
      : m_token(token), m_value(value) {}
  TokenValuePair(const Token &token) : m_token(token), m_value(std::nullopt) {}
  TokenValuePair(const Token &token, std::optional<TokenValue> &&value)
      : m_token(token), m_value(value) {}

  operator TokenType() const { return m_token.getTokenType(); }
};

class ITokenGenerator : public ILoggable {
public:
  virtual TokenValuePair getNext() = 0;
};
}; // namespace rgl
