#pragma once
#include "common/Formatter.hpp"
#include "common/ILoggable.hpp"
#include "common/errors/ErrorUtilities.hpp"
#include "lexer/Token.hpp"
#include "lexer/TokenCollection.hpp"

#include <memory>

namespace rgl {
class ErrorObject : public ILoggable {
public:
  ErrorObject(std::string message)
      : m_message(std::move(message)), m_tok(Token(TokenType::t_err)) {}
  ErrorObject(std::string message, std::string hint)
      : m_message(std::move(message)), m_hint(std::move(hint)) {}
  ErrorObject(std::string message, Token tok, std::string hint = "")
      : m_message(std::move(message)), m_hint(std::move(hint)), m_tok(tok) {}
  ErrorObject(std::string message,
              const std::unique_ptr<TokenCollection> &tokenCollection,
              std::string hint = "")
      : m_message(std::move(message)), m_hint(std::move(hint)),
        m_tok(tokenCollection->getCurr()) {}

  virtual std::string toString() const override {
    if (0 == m_hint.size() && TokenType::t_err == m_tok) {
      return Formatter("{}", m_message);
    } else if (0 == m_hint.size()) {
      return Formatter("{}\n{}", m_message, pointAt(m_tok));
    } else if (TokenType::t_err == m_tok) {
      return Formatter("{}\n    (HINT: {})", m_message, m_hint);
    }
    return Formatter("{}\n    (HINT: {})\n{}", m_message, m_hint,
                     pointAt(m_tok));
  }

private:
  const std::string m_message;
  const std::string m_hint;
  const Token m_tok;
};
}; // namespace rgl
