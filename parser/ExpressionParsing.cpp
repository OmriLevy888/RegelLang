#include "common/errors/ErrorManager.hpp"
#include "common/errors/ErrorObject.hpp"
#include "common/errors/ErrorUtilities.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ParserUtilities.hpp"

#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

#include "parser/ast/expressions/SwitchCaseNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/CharLiteralNode.hpp"
#include "parser/ast/expressions/literals/DoubleLiteralNode.hpp"
#include "parser/ast/expressions/literals/FloatLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"
#include "parser/ast/expressions/literals/StringLiteralNode.hpp"
#include "parser/ast/expressions/literals/UintLiteralNode.hpp"

#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/IndexNode.hpp"
#include "parser/ast/expressions/ops/InvokeNode.hpp"
#include "parser/ast/expressions/ops/ParenthesesNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"

#include "parser/ast/expressions/ConditionalNode.hpp"
#include "parser/ast/expressions/ForInLoopNode.hpp"
#include "parser/ast/expressions/ForLoopNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/WhileLoopNode.hpp"

#include <memory>

namespace rgl {
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
    } else if (TokenType::t_open_bracket == m_tokens->getCurr()) {
      return parseBlock();
    } else if (TokenType::t_func == m_tokens->getCurr()) {
      return parseFunction();
    }

    return parseImplicitStatementExpression();
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
  } else if (TokenType::t_open_paren == curr) {
    return parseInvoke(std::move(primary));
  } else if (TokenType::t_open_square == curr) {
    return parseIndex(std::move(primary));
  } else if (ParserUtilities::isPostOp(curr)) {
    return parsePostOp(std::move(primary));
  }

  return primary;
}

Expression Parser::parseImplicitStatementExpression() {
  if (TokenType::t_if == m_tokens->getCurr()) {
    return parseConditional();
  } else if (TokenType::t_for == m_tokens->getCurr()) {
    return parseForLoop();
  } else if (TokenType::t_while == m_tokens->getCurr()) {
    return parseWhileLoop();
  } else if (TokenType::t_switch == m_tokens->getCurr()) {
    return parseSwitch();
  }

  ErrorManager::logError(
      ErrorTypes::E_BAD_TOKEN,
      {Formatter("Expected expression but found {}", tokenToString(m_tokens)),
       m_tokens});
  return nullptr;
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
      ErrorManager::logError(
          ErrorTypes::E_BAD_TOKEN,
          {Formatter("Expected ), found {}", tokenToString(m_tokens)), m_tokens,
           "Did you forget the closing parenthesis?"});
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

Expression Parser::parsePostOp(Expression primary) {
  UnaryOpType opType = ParserUtilities::tokToPostOpType(m_tokens->getCurr());
  m_tokens->getNext(); // consume post-op
  auto postOpExpr = std::make_unique<UnaryOpNode>(opType, std::move(primary));
  return parseRest(std::move(postOpExpr));
}

Expression Parser::parseInvoke(Expression primary) {
  m_tokens->getNext(); // consume (

  std::vector<Expression> params;
  bool hadComma = false;

  if (TokenType::t_close_paren != m_tokens->getCurr()) {
    do {
      hadComma = false;
      auto param = parseExprssion();
      if (nullptr == param) {
        // TODO: write error message
        return nullptr;
      }
      params.push_back(std::move(param));

      if (TokenType::t_comma == m_tokens->getCurr()) {
        hadComma = true;
        m_tokens->getNext();
      }
    } while (TokenType::t_close_paren != m_tokens->getCurr() && hadComma);
  }

  if (TokenType::t_close_paren != m_tokens->getCurr()) {
    ErrorManager::logError(
        ErrorTypes::E_BAD_TOKEN,
        {Formatter("Expected ), found {}", tokenToString(m_tokens)), m_tokens,
         "Did you forget the closing parenthesis?"});
    return nullptr;
  }
  m_tokens->getNext(); // consume )

  return parseRest(
      std::make_unique<InvokeNode>(std::move(primary), std::move(params)));
}

Expression Parser::parseIndex(Expression primary) {
  m_tokens->getNext(); // consume [

  auto index = parseExprssion();
  if (nullptr == index) {
    // TODO: write error message
    return nullptr;
  }

  if (TokenType::t_close_square != m_tokens->getCurr()) {
    ErrorManager::logError(
        ErrorTypes::E_BAD_TOKEN,
        {Formatter("Expected ], found {}", tokenToString(m_tokens)), m_tokens,
         "Did you forget the closing square bracket?"});
    return nullptr;
  }
  m_tokens->getNext(); // consume ]

  return parseRest(
      std::make_unique<IndexNode>(std::move(primary), std::move(index)));
}

Expression Parser::parseVarDecl() {
  bool isMutable = TokenType::t_var == m_tokens->getCurr();
  if (TokenType::t_identifier != m_tokens->getNext()) {
    // TODO: write error message
    return nullptr;
  }
  auto name = parseIdentifier();
  m_tokens->getNext(); // consume identifier

  TypePtr type = Type::t_implicit();
  if (TokenType::t_colon == m_tokens->getCurr()) { // parse type
    m_tokens->getNext();                           // consume :
    type = parseType(true);
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
  } else if (!isMutable) { // let must have a value
    ErrorManager::logError(
        ErrorTypes::E_BAD_TOKEN,
        {Formatter("Expected initial value for constant, found {}",
                   tokenToString(m_tokens)),
         m_tokens});
    return nullptr;
  }

  if (isMutable) {
    type = type->getMutableType();
  } else {
    type = type->getOwningType();
  }

  return std::make_unique<VarDeclNode>(std::move(name), type, std::move(expr));
}

Block Parser::parseBlock() {
  // TODO: implement single line block
  bool isSingleStatement = TokenType::t_open_bracket != m_tokens->getCurr();
  std::vector<Statement> statements;

  if (!isSingleStatement) {
    Token openBracket = m_tokens->getCurr();
    m_tokens->getNext(); // consume {
    while (TokenType::t_close_bracket != m_tokens->getCurr() &&
           TokenType::t_eof != m_tokens->getCurr()) {
      auto curr = parseStatement();
      if (nullptr == curr) {
        // TODO: write error message
        return nullptr;
      }
      statements.push_back(std::move(curr));
    }

    if (TokenType::t_close_bracket != m_tokens->getCurr()) {
      ErrorManager::logError(ErrorTypes::E_BAD_TOKEN,
                             {"Expected } but not found", openBracket,
                              m_tokens->getSourceProject()});
      return nullptr;
    }
    m_tokens->getNext(); // consume }
  } else {
    auto statement = parseStatement();
    if (nullptr == statement) {
      // TODO: write error message
      return nullptr;
    }
    statements.push_back(std::move(statement));
  }
  return std::make_unique<BlockNode>(std::move(statements));
}

Expression Parser::parseConditional() {
  m_tokens->getNext(); // consume if
  Expression cond = parseExprssion();
  if (nullptr == cond) {
    // TODO: write error message
    return nullptr;
  }

  Block body = parseBlock();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  Expression _else;
  if (TokenType::t_elif == m_tokens->getCurr()) {
    _else = parseConditional();
    if (nullptr == _else) {
      // TODO: write error message
      return nullptr;
    }
  } else if (TokenType::t_else == m_tokens->getCurr()) {
    if (TokenType::t_if == m_tokens->getNext()) {
      _else = parseConditional();
      if (nullptr == _else) {
        // TODO: write error message
        return nullptr;
      }
    } else {
      _else = parseBlock();
      if (nullptr == _else) {
        // TODO: write error message
        return nullptr;
      }
    }
  }

  return std::make_unique<ConditionalNode>(std::move(cond), std::move(body),
                                           std::move(_else));
}

Expression Parser::parseForLoop() {
  m_tokens->saveAnchor();
  m_tokens->getNext(); // consume for

  if (TokenType::t_identifier == m_tokens->getCurr()) {
    auto forInLoop = parseForInLoop();
    if (nullptr != forInLoop) {
      m_tokens->discardAnchor();
      return forInLoop;
    }
    m_tokens->restoreAnchor();
  } else {
    m_tokens->discardAnchor();
  }

  Expression init;
  if (TokenType::t_semicolon != m_tokens->getCurr()) {
    init = parseExprssion();
    if (nullptr == init) {
      ErrorManager::getErrorType(); // consume error
      return parseForInLoop();
    }
  }

  if (TokenType::t_semicolon != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume ;

  Expression cond;
  if (TokenType::t_semicolon != m_tokens->getCurr()) {
    cond = parseExprssion();
    if (nullptr == cond) {
      // TODO: write error message
      return nullptr;
    }
  }
  if (TokenType::t_semicolon != m_tokens->getCurr()) {
    // TODO: wrtite error message
    return nullptr;
  }
  m_tokens->getNext(); // consume ;

  Expression advance;
  if (TokenType::t_open_bracket != m_tokens->getCurr()) {
    advance = parseExprssion();
    if (nullptr == advance) {
      // TODO: write error message
      return nullptr;
    }
  }

  auto body = parseBlock();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  return std::make_unique<ForLoopNode>(std::move(init), std::move(cond),
                                       std::move(advance), std::move(body));
}

Expression Parser::parseForInLoop() {
  PassType type = PassType::p_const;
  if (TokenType::t_identifier != m_tokens->getCurr()) {
    switch (m_tokens->getCurr()) {
    case TokenType::t_colon:
      type = PassType::p_consume;
      break;
    case TokenType::t_ampersand:
      type = PassType::p_mutable;
      break;
    default:
      // TODO: write error message
      return nullptr;
    }
    m_tokens->getNext(); // consume pass type specifier
  }

  if (TokenType::t_identifier != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  Identifier name = parseIdentifier();
  m_tokens->getNext(); // consume identifier

  if (TokenType::t_in != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume in

  Expression iterrable = parseExprssion();
  if (nullptr == iterrable) {
    // TODO: write error message
    return nullptr;
  }

  if (TokenType::t_semicolon == m_tokens->getCurr()) {
    return nullptr;
  }

  Block body = parseBlock();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  return std::make_unique<ForInLoopNode>(type, std::move(name),
                                         std::move(iterrable), std::move(body));
}

Expression Parser::parseWhileLoop() {
  m_tokens->getNext(); // consume while

  Expression cond;
  if (TokenType::t_open_bracket != m_tokens->getCurr()) {
    cond = parseExprssion();
    if (nullptr == cond) {
      // TODO: write error message
      return nullptr;
    }
  }

  Block body = parseBlock();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  return std::make_unique<WhileLoopNode>(std::move(cond), std::move(body));
}

Switch Parser::parseSwitch() {
  m_tokens->getNext(); // consume switch keyword

  auto expr = parseExprssion();
  if (nullptr == expr) {
    // TODO: write error message
    return nullptr;
  }

  TypePtr caseExprType;
  if (TokenType::t_colon == m_tokens->getCurr()) {
    m_tokens->getNext(); // consume :
    caseExprType = parseType();
    if (nullptr == caseExprType) {
      // TODO: write error message
      return nullptr;
    }
  }

  if (TokenType::t_open_bracket != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume {

  std::vector<SwitchCase> cases;
  while (TokenType::t_close_bracket != m_tokens->getCurr()) {
    auto currCase = parseSwitchCase();
    if (nullptr == currCase) {
      // TODO: write error message
      return nullptr;
    }
    cases.push_back(std::move(currCase));
  }
  m_tokens->getNext(); // consume }

  return std::make_unique<SwitchNode>(std::move(expr), caseExprType,
                                      std::move(cases));
}

SwitchCase Parser::parseSwitchCase() {
  auto expr = parseExprssion();
  if (nullptr == expr) {
    if (TokenType::t_underscore != m_tokens->getCurr()) {
      // TODO: write error message
      return nullptr;
    }
  }

  if (TokenType::t_arrow != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume =>

  auto body = parseBlock();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  return std::make_unique<SwitchCaseNode>(std::move(expr), std::move(body));
}

Expression Parser::parseFunction() {
  Identifier name = nullptr;
  if (TokenType::t_identifier == m_tokens->getNext()) {
    name = parseIdentifier();
    m_tokens->getNext(); // consume name
  }

  // parse parameters
  std::vector<Parameter> parameters;
  const bool multipleParams = TokenType::t_open_paren == m_tokens->getCurr();
  if (multipleParams) {
    m_tokens->getNext(); // consume (
    TypePtr lastType = nullptr;
    while (TokenType::t_close_paren != m_tokens->getCurr()) {
      // TODO: disable the ErrorManager
      TypePtr paramType = parseType();
      if (nullptr == paramType && nullptr == lastType) {
        // TODO: write error message
        return nullptr;
      }

      if (TokenType::t_identifier != m_tokens->getCurr()) {
        // TODO: write error message
        return nullptr;
      }
      Identifier paramName = parseIdentifier();
      parameters.push_back(
          std::make_unique<ParameterNode>(paramType, std::move(paramName)));

      if (TokenType::t_comma != m_tokens->getNext()) {
        break;
      }
    }

    if (TokenType::t_close_paren != m_tokens->getCurr()) {
      // TODO: write error message
      return nullptr;
    }
  } else if (TokenType::t_arrow != m_tokens->getCurr()) {
    // if there is only one param
    TypePtr paramType = parseType();
    if (nullptr == paramType) {
      return nullptr;
    }

    if (TokenType::t_identifier != m_tokens->getCurr()) {
      // TODO: write error message
      return nullptr;
    }
    Identifier paramName = parseIdentifier();
    m_tokens->getNext();
    parameters.push_back(
        std::make_unique<ParameterNode>(paramType, std::move(paramName)));
  }

  // parse retType
  TypePtr retType = nullptr;
  if (TokenType::t_arrow == m_tokens->getCurr()) {
    retType = parseType();
    if (nullptr == retType) {
      // TODO: write error message
      return nullptr;
    }
  }

  // parse body
  Expression body = parseExprssion();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  return nullptr;
}
}; // namespace rgl
