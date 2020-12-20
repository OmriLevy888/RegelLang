#pragma once
#include "common/Core.hpp"
#include "lexer/ITokenGenerator.hpp"

#include <stack>
#include <vector>

namespace rgl {
class TokenCollection : public ILoggable {
public:
  TokenCollection(std::unique_ptr<ITokenGenerator> &&tokenGenerator)
      : m_baseIndex(0), m_backlogIndex(0), m_actualIndex(0),
        m_usingBacklog(false), m_tokenGenerator(std::move(tokenGenerator)),
        m_backlog(), m_value(std::nullopt) {}

  void saveAnchor();
  void restoreAnchor();
  size_t peekAnchor();
  const Token &getNext();
  const Token &getCurr() { return m_curr; }
  std::optional<TokenValue> &getCurrValue() { return m_value; }

  std::string toString() const override {
    return "TokenCollection<baseIndex: " + std::to_string(m_baseIndex) +
           ", backlogIndex: " + std::to_string(m_backlogIndex) +
           ", actualIndex: " + std::to_string(m_actualIndex) +
           ", usingBacklog: " + std::to_string(m_usingBacklog) + ">";
  }

private:
  std::unique_ptr<ITokenGenerator> m_tokenGenerator;
  Token m_curr;
  std::optional<TokenValue> m_value;

  size_t m_baseIndex;
  size_t m_backlogIndex;
  size_t m_actualIndex;
  bool m_usingBacklog;
  std::stack<size_t> m_anchors;
  std::vector<Token> m_backlog;
};
} // namespace rgl
