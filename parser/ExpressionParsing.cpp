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

#include <memory>

namespace rgl {
Expression Parser::parseExprssion() {
  m_lastPrecedence = 0;
  auto primary = parsePrimary();
  if (nullptr == primary) {
    auto parenExpr = parseParentheses();
    if (nullptr != parenExpr) {
      return parenExpr;
    }
    // NOTE: else case could be used for prefixed unary operations
    // TODO: write error message
    return nullptr;
  }

  const Token &next = m_tokens->getNext();
  if (ParserUtilities::isBinOp(next)) { // parser bin-op
    return parseBinOp(std::move(primary));
  }

  // just a primary
  return primary;
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

  return primary;
}

Expression Parser::parseIdentifier() {
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
  const Token &op = m_tokens->getCurr();
  m_tokens->getNext(); // consume bin-op
  auto rhs = parseExprssion();
  if (nullptr == rhs) {
    // TODO: write error message
    return nullptr;
  }

  uint8_t currPrecedence = ParserUtilities::tokToPrecedence(op);
  BinOpType binOpType = ParserUtilities::tokToBinOpType(op);
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
}; // namespace rgl
