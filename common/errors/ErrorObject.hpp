#pragma once
#include "common/Formatter.hpp"
#include "common/ILoggable.hpp"
#include "common/errors/ErrorUtilities.hpp"
#include "common/source-objects/SourceRange.hpp"
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
  ErrorObject(std::string message, const SourceRange &range,
              std::string hint = "")
      : m_message(std::move(message)), m_hint(std::move(hint)),
        m_tok(Token(TokenType::t_err)), m_range(range) {}

  virtual std::string toString() const override;

private:
  const std::string m_message;
  const std::string m_hint;
  const Token m_tok;
  const SourceRange m_range;
};
}; // namespace rgl
