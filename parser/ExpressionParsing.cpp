#include "common/errors/ErrorManager.hpp"
#include "parser/Parser.hpp"
#include "parser/ParserUtilities.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteral.hpp"
#include "parser/ast/expressions/literals/CharLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/RealLiteralNode.hpp"
#include "parser/ast/expressions/literals/StringLiteralNode.hpp"
#include <debug/assertions.h>

namespace rgl {
Expression Parser::parseExprssion() {
  std::unique_ptr<ExpressionNode> primary;
  const Token &tok = m_tokens->getNext();

  if (ParserUtilities::isIdentifier(tok)) {
    primary = parseIdentifier();
  } else if (ParserUtilities::isLiteral(tok)) {
    primary = parseLiteral();
  }

  // Unexpected token
  return nullptr;
}

Expression Parser::parseIdentifier() {
  _GLIBCXX_DEBUG_ASSERT(m_tokens->getCurrValue().has_value());
  auto value = std::get<std::string>(m_tokens->getCurrValue().value());
  return std::make_unique<IdentifierNode>(std::move(value));
}

Expression Parser::parseLiteral() { return nullptr; }

Expression Parser::parseIntLiteral() { return nullptr; }

Expression Parser::parseRealLiteral() { return nullptr; }

Expression Parser::parseTextLiteral() { return nullptr; }

Expression Parser::parseBoolLiteral() {
  _GLIBCXX_DEBUG_ASSERT(m_tokens->getCurrValue().has_value());
  bool value = std::get<bool>(m_tokens->getCurrValue().value());
  return std::make_unique<BooleanLiteral>(value);
}
}; // namespace rgl
