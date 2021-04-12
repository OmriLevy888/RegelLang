#include "common/errors/ErrorManager.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include <vector>

namespace rgl {
ClassPtr Parser::parseClass() {
  if (TokenType::t_class != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume `class`

  Identifier name = nullptr; // indicates an anonymous class
  if (TokenType::t_identifier == m_tokens->getCurr()) {
    name = parseIdentifier();
    m_tokens->getNext(); // consume identifier
  }

  if (TokenType::t_open_bracket != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume {

  std::vector<FieldPtr> fields{};
  std::vector<MethodPtr> methods{};

  while (TokenType::t_close_bracket != m_tokens->getCurr()) {
    bool isExposed = (TokenType::t_pub == m_tokens->getCurr());
    if (isExposed) {
      m_tokens->getNext(); // consume `pub`
    }

    switch (m_tokens->getCurr()) {
    case TokenType::t_func:
      if (!parseMethod(methods)) {
        return nullptr;
      }
      break;
    case TokenType::t_hook:
      // TODO: implement
      ErrorManager::logError(ErrorTypes::E_NOT_YET_IMPLEMENTED,
                             "Hooks are not implemented yet");
      return nullptr;
    default:
      if (!parseField(fields)) {
        return nullptr;
      }
      break;
    }
  }

  return std::make_unique<ClassLiteralNode>(std::move(name), std::move(fields),
                                            std::move(methods));
}

bool Parser::parseField(std::vector<FieldPtr> &fiels) { return false; }

bool Parser::parseMethod(std::vector<MethodPtr> &method) { return false; }
}; // namespace rgl
