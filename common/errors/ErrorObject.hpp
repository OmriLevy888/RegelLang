#pragma once
#include "common/Formatter.hpp"
#include "common/ILoggable.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "common/errors/ErrorUtilities.hpp"
#include "lexer/Token.hpp"
#include "lexer/TokenCollection.hpp"

#include <memory>

namespace rgl {
class ErrorObject : public ILoggable {
public:
  ErrorObject(std::string message)
      : m_message(message), m_tok(Token(TokenType::t_err)) {}
  ErrorObject(std::string message, Token tok,
              std::shared_ptr<SourceProject> sourceProject,
              std::string hint = "")
      : m_message(message), m_tok(tok), m_sourceProject(sourceProject),
        m_hint(hint) {}
  ErrorObject(std::string message,
              const std::unique_ptr<TokenCollection> &tokenCollection,
              std::string hint = "")
      : m_message(message), m_tok(tokenCollection->getCurr()),
        m_sourceProject(tokenCollection->getSourceProject()), m_hint(hint) {}

  virtual std::string toString() const override {
    if (0 == m_hint.size() && TokenType::t_err == m_tok) {
      return Formatter("{}", m_message);
    } else if (0 == m_hint.size()) {
      return Formatter("{}\n{}", m_message, pointAt(m_tok, m_sourceProject));
    } else if (TokenType::t_err == m_tok) {
      return Formatter("{}\n    (HINT: {})", m_message, m_hint);
    }
    return Formatter("{}\n    (HINT: {})\n{}", m_message, m_hint,
                     pointAt(m_tok, m_sourceProject));
  }

private:
  const std::string m_message;
  const std::string m_hint;
  const Token m_tok;
  const std::shared_ptr<SourceProject> m_sourceProject;
};
}; // namespace rgl
