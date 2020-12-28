#include "common/errors/ErrorManager.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ParserUtilities.hpp"

#include "parser/ast/expressions/IdentifierNode.hpp"

#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/CharLiteralNode.hpp"
#include "parser/ast/expressions/literals/DoubleLiteralNode.hpp"
#include "parser/ast/expressions/literals/FloatLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/StringLiteralNode.hpp"
#include "parser/ast/expressions/literals/UintLiteralNode.hpp"

#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/ParenthesesNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"

#include "parser/ast/expressions/VarDeclNode.hpp"

#include <memory>

namespace rgl {
TypePtr Parser::parseType() {
  const bool isReference = TokenType::t_ampersand == m_tokens->getCurr();
  if (isReference) {
    m_tokens->getNext(); // consume &
  }

  std::vector<std::string> name;
  if (!ParserUtilities::isIdentifier(m_tokens->getCurr())) {
    // TODO: write error message
    return nullptr;
  }
  name.push_back(
      std::get<std::string>(std::move(m_tokens->getCurrValue().value())));
  m_tokens->getNext(); // consume first identifier

  while (TokenType::t_dot == m_tokens->getCurr()) {
    if (TokenType::t_identifier != m_tokens->getNext()) {
      // TODO: write error message
      return nullptr;
    }

    name.push_back(
        std::get<std::string>(std::move(m_tokens->getCurrValue().value())));
    m_tokens->getNext(); // consume current identifier
  }

  return makeType(std::move(name), isReference);
}

Expression Parser::parseExprssion() {
  m_lastPrecedence = 0;
  auto primary = parsePrimary();
  if (nullptr == primary) {
    if (ParserUtilities::isPreOp(m_tokens->getCurr())) {
      return parsePreOp();
    } else if (TokenType::t_open_paren == m_tokens->getCurr()) {
      return parseParentheses();
    } else if (ParserUtilities::isVarDecl(m_tokens->getCurr())) {
      return parseVarDecl();
    }
    // NOTE: else case could be used for prefixed unary operations
    // TODO: write error message
    return nullptr;
  }

  return parseRest(std::move(primary));
}

Expression Parser::parsePrimary() {
  Expression primary;
  const Token &tok = m_tokens->getCurr();

  if (ParserUtilities::isIdentifier(tok)) {
    primary = parseIdentifier();
  } else if (ParserUtilities::isLiteral(tok)) {
    primary = parseLiteral();
  } else {
    return nullptr;
  }

  m_tokens->getNext(); // consume current token
  return primary;
}

Expression Parser::parseRest(Expression primary) {
  const Token &curr = m_tokens->getCurr();
  if (ParserUtilities::isBinOp(curr)) {
    return parseBinOp(std::move(primary));
  } else if (ParserUtilities::isPostOp(curr)) {
    return parsePostOp(std::move(primary));
  }

  return primary;
}

std::unique_ptr<IdentifierNode> Parser::parseIdentifier() {
  return std::make_unique<IdentifierNode>(std::move(
      std::get<std::string>(std::move(m_tokens->getCurrValue().value()))));
}

Expression Parser::parseLiteral() {
  const Token &tok = m_tokens->getCurr();
  if (ParserUtilities::isIntLiteral(tok)) {
    return parseIntLiteral();
  } else if (ParserUtilities::isRealLiteral(tok)) {
    return parseRealLiteral();
  } else if (ParserUtilities::isTextLiteral(tok)) {
    return parseTextLiteral();
  } else if (ParserUtilities::isBooleanLiteral(tok)) {
    return parseBoolLiteral();
  }
  return nullptr;
}

Expression Parser::parseIntLiteral() {
  if (ParserUtilities::isSignedIntLiteral(m_tokens->getCurr())) {
    std::shared_ptr<Type> intType;
    switch (m_tokens->getCurr()) {
    case TokenType::t_int8_literal:
      intType = Type::t_int8();
      break;
    case TokenType::t_int16_literal:
      intType = Type::t_int16();
      break;
    case TokenType::t_int32_literal:
      intType = Type::t_int32();
      break;
    case TokenType::t_int64_literal:
      intType = Type::t_int64();
      break;
    default:
      return nullptr;
    }
    int64_t intValue = std::get<int64_t>(m_tokens->getCurrValue().value());
    return std::make_unique<IntLiteralNode>(intValue, intType);
  } else if (ParserUtilities::isUnsignedIntLiteral(m_tokens->getCurr())) {
    std::shared_ptr<Type> uintType;
    switch (m_tokens->getCurr()) {
    case TokenType::t_uint8_literal:
      uintType = Type::t_uint8();
      break;
    case TokenType::t_uint16_literal:
      uintType = Type::t_uint16();
      break;
    case TokenType::t_uint32_literal:
      uintType = Type::t_uint32();
      break;
    case TokenType::t_uint64_literal:
      uintType = Type::t_uint64();
      break;
    default:
      return nullptr;
    }
    uint64_t uintValue = std::get<uint64_t>(m_tokens->getCurrValue().value());
    return std::make_unique<UintLiteralNode>(uintValue, uintType);
  }

  return nullptr;
}

Expression Parser::parseRealLiteral() {
  const Token &curr = m_tokens->getCurr();
  if (curr == TokenType::t_float_literal) {
    float floatValue = std::get<float>(m_tokens->getCurrValue().value());
    return std::make_unique<FloatLiteralNode>(floatValue);
  } else if (curr == TokenType::t_double_literal) {
    double doubleValue = std::get<double>(m_tokens->getCurrValue().value());
    return std::make_unique<DoubleLiteralNode>(doubleValue);
  }

  return nullptr;
}

Expression Parser::parseTextLiteral() {
  switch (m_tokens->getCurr().getTokenType()) {
  case TokenType::t_char_literal:
    return std::make_unique<CharLiteralNode>(
        std::get<char>(m_tokens->getCurrValue().value()));
  case TokenType::t_string_literal:
    return std::make_unique<StringLiteralNode>(std::move(
        std::get<std::string>(std::move(m_tokens->getCurrValue().value()))));
  default:
    return nullptr;
  }
}

Expression Parser::parseBoolLiteral() {
  bool value = std::get<bool>(m_tokens->getCurrValue().value());
  return std::make_unique<BooleanLiteralNode>(value);
}

Expression Parser::parseParentheses() {
  if (TokenType::t_open_paren == m_tokens->getCurr()) {
    m_tokens->getNext(); // consume (
    auto innerExpr = parseExprssion();
    if (nullptr == innerExpr) {
      // TODO: write error message
      return nullptr;
    } else if (TokenType::t_close_paren != m_tokens->getCurr()) {
      // TODO: write error message
      return nullptr;
    }

    m_lastPrecedence = 0;
    return std::make_unique<ParenthesesNode>(std::move(innerExpr));
  }
  return nullptr;
}

Expression Parser::parseBinOp(Expression primary) {
  const Token op = m_tokens->getCurr();
  m_tokens->getNext(); // consume bin-op
  auto rhs = parseExprssion();
  if (nullptr == rhs) {
    // TODO: write error message
    return nullptr;
  }

  const uint8_t currPrecedence = ParserUtilities::tokToPrecedence(op);
  const BinOpType binOpType = ParserUtilities::tokToBinOpType(op);
  auto binOp =
      std::make_unique<BinOpNode>(binOpType, std::move(primary), nullptr);
  if (currPrecedence <= m_lastPrecedence) { // should switch
    rhs->propagateLeft(std::move(binOp));
    m_lastPrecedence = currPrecedence;
    return rhs;
  }

  m_lastPrecedence = currPrecedence;
  binOp->setSwap(std::move(rhs));
  return binOp;
}

Expression Parser::parsePreOp() {
  UnaryOpType opType = ParserUtilities::tokToPreOpType(m_tokens->getCurr());
  m_tokens->getNext(); // consume pre-op
  auto expr = parseExprssion();
  if (nullptr == expr) {
    // TODO: write error message
    return nullptr;
  }

  const size_t currPrecedence = ParserUtilities::getPreOpPrecedence();
  if (currPrecedence > m_lastPrecedence) {
    m_lastPrecedence = currPrecedence;
    return std::make_unique<UnaryOpNode>(opType, std::move(expr));
  }

  auto unaryOp = std::make_unique<UnaryOpNode>(opType, nullptr);
  m_lastPrecedence = currPrecedence;
  expr->propagateLeft(std::move(unaryOp));
  return expr;
}

Expression Parser::parsePostOp(Expression expr) {
  UnaryOpType opType = ParserUtilities::tokToPostOpType(m_tokens->getCurr());
  m_tokens->getNext(); // consume post-op
  auto primary = std::make_unique<UnaryOpNode>(opType, std::move(expr));
  return parseRest(std::move(primary));
}

Expression Parser::parseVarDecl() {
  bool isConst = TokenType::t_let == m_tokens->getCurr();
  m_tokens->getNext(); // consume let/var
  auto name = parseIdentifier();
  if (nullptr == name) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume identifier

  TypePtr type;
  if (TokenType::t_colon == m_tokens->getCurr()) { // parse type
    m_tokens->getNext();                           // consume :
    type = parseType();
    if (nullptr == type) {
      // TODO: wrrite error message
      return nullptr;
    }
  }

  Expression expr;
  if (TokenType::t_equal == m_tokens->getCurr()) { // parse initial value
    m_tokens->getNext();                           // consume =

    expr = parseExprssion();
    if (nullptr == expr) {
      // TODO: write error message
      return nullptr;
    }
  }

  return std::make_unique<VarDeclNode>(std::move(name), type, isConst,
                                       std::move(expr));
}
}; // namespace rgl
