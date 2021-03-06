#include "common/errors/ErrorManager.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ParserUtilities.hpp"

#include "parser/ast/statements/BreakNode.hpp"
#include "parser/ast/statements/ContinueNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/ReturnNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"

#include <memory>

namespace rgl {
Statement Parser::parseStatement() {
  if (TokenType::t_semicolon == m_tokens->getCurr()) {
    // TODO: add warning of empty statement
    m_tokens->getNext(); // consume ;
    return std::make_unique<ExpressionStatementNode>();
  }

  if (ParserUtilities::isKeywordStatement(m_tokens->getCurr())) {
    return parseKeywordStatement();
  } else if (ParserUtilities::isSimpleStatement(m_tokens->getCurr())) {
    return parseSimpleStatement();
  } else if (ParserUtilities::isImplicitlyStatementExpression(
                 m_tokens->getCurr())) {
    auto implExpr = parseImplicitStatementExpression();
    if (nullptr == implExpr) {
      return nullptr;
    }

    if (TokenType::t_semicolon == m_tokens->getCurr()) {
      m_tokens->getNext(); // consume ;
    }

    return std::make_unique<ExpressionStatementNode>(std::move(implExpr));
  } else {
    auto expr = parseExpression();
    if (nullptr == expr) {
      return nullptr;
    } else if (TokenType::t_semicolon != m_tokens->getCurr()) {
      ErrorManager::logError(
          ErrorTypes::E_BAD_TOKEN,
          {Formatter("Expected ; found {}", tokenToString(m_tokens)),
           m_tokens});
      return nullptr;
    }
    m_tokens->getNext(); // consume semicolon

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
    ErrorManager::logError(
        ErrorTypes::E_BAD_TOKEN,
        {Formatter("Expected ; found {}", tokenToString(m_tokens)), m_tokens});
    return nullptr;
  }

  m_tokens->getNext();
  return ret;
};

Statement Parser::parseSimpleStatement() {
  bool isReturn = TokenType::t_return == m_tokens->getCurr();

  Expression expr;
  if (TokenType::t_semicolon != m_tokens->getNext()) {
    expr = parseExpression();
    if (nullptr == expr) {
      return nullptr;
    }
  }

  if (TokenType::t_semicolon != m_tokens->getCurr()) {
    ErrorManager::logError(
        ErrorTypes::E_BAD_TOKEN,
        {Formatter("Expected ; found {}", tokenToString(m_tokens)), m_tokens});
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
