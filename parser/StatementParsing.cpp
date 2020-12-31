#include "common/errors/ErrorManager.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ParserUtilities.hpp"

#include "parser/ast/statement/BreakNode.hpp"
#include "parser/ast/statement/ContinueNode.hpp"
#include "parser/ast/statement/ExpressionStatementNode.hpp"
#include "parser/ast/statement/ReturnNode.hpp"
#include "parser/ast/statement/YieldNode.hpp"

#include <memory>

namespace rgl {
Statement Parser::parseStatement() {
  auto statement = parseKeywordStatement();
  if (nullptr != statement) {
    m_tokens->getNext(); // consume keyword
    return statement;
  } else if (ParserUtilities::isSimpleStatement(m_tokens->getCurr())) {
    auto ret = parseSimpleStatement();
    return ret;
  } else {
    auto expr = parseExprssion();
    if (nullptr == expr) {
      // TODO: write error message
      return nullptr;
    }

    return std::make_unique<ExpressionStatementNode>(std::move(expr));
  }
}

Statement Parser::parseKeywordStatement() {
  Statement ret;
  switch (m_tokens->getCurr()) {
  case TokenType::t_continue:
    ret = std::make_unique<ContinueNode>();
    break;
  case TokenType::t_break:
    ret = std::make_unique<BreakNode>();
    break;
  default:
    return nullptr;
  }

  if (TokenType::t_semicolon != m_tokens->getNext()) {
    // TODO: write error message
    return nullptr;
  }
  return ret;
};

Statement Parser::parseSimpleStatement() {
  bool isReturn = TokenType::t_return == m_tokens->getCurr();

  Expression expr;
  if (TokenType::t_semicolon != m_tokens->getNext()) {
    expr = parseExprssion();
    if (nullptr == expr) {
      // TODO: write error message
      return nullptr;
    }
  }

  if (TokenType::t_semicolon != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume semicolon

  if (isReturn) {
    return std::make_unique<ReturnNode>(std::move(expr));
  } else {
    return std::make_unique<YieldNode>(std::move(expr));
  }
}
}; // namespace rgl
