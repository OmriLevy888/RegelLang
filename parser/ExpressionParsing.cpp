#include <memory>

#include "common/errors/ErrorManager.hpp"
#include "common/errors/ErrorObject.hpp"
#include "common/errors/ErrorUtilities.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ParserUtilities.hpp"
#include "parser/ast/constructs/BasicTypeNode.hpp"
#include "parser/ast/expressions/ConditionalNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/ForInLoopNode.hpp"
#include "parser/ast/expressions/ForLoopNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/ScopeNode.hpp"
#include "parser/ast/expressions/SwitchCaseNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/WhileLoopNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/CharLiteralNode.hpp"
#include "parser/ast/expressions/literals/DoubleLiteralNode.hpp"
#include "parser/ast/expressions/literals/FloatLiteralNode.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"
#include "parser/ast/expressions/literals/StringLiteralNode.hpp"
#include "parser/ast/expressions/literals/UintLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/IndexNode.hpp"
#include "parser/ast/expressions/ops/InvokeNode.hpp"
#include "parser/ast/expressions/ops/ParenthesesNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"

namespace rgl {
Expression Parser::parseExpression(bool allowFunctionName) {
  m_lastPrecedence = 0;
  auto primary = parsePrimary();
  if (nullptr == primary) {
    if (ParserUtilities::isPreOp(m_tokens->getCurr())) {
      return parsePreOp();
    }

    switch (m_tokens->getCurr()) {
    case TokenType::t_open_paren:
      return parseParentheses();
    case TokenType::t_var:
      return parseVarDecl();
    case TokenType::t_open_bracket:
      return parseScope();
    case TokenType::t_func:
      return parseFunction(allowFunctionName);
    case TokenType::t_class:
      return parseClass();
    case TokenType::t_import:
      return parseImport();
    default:
      return parseImplicitStatementExpression();
    }
  }

  return parseRest(std::move(primary));
}

Expression Parser::parsePrimary() {
  const Token &tok = m_tokens->getCurr();

  if (ParserUtilities::isIdentifier(tok)) {
    auto identifier = parseIdentifier();
    if (TokenType::t_colon == m_tokens->getCurr()) {
      return parseVarDecl(std::move(identifier));
    }
    return identifier;
  } else if (ParserUtilities::isLiteral(tok)) {
    auto literal = parseLiteral();
    m_tokens->getNext(); // consume current token
    return literal;
  }

  return nullptr;
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

Identifier Parser::parseIdentifier() {
  if (TokenType::t_identifier != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }

  std::vector<std::string> parts{};
  parts.push_back(std::get<std::string>(m_tokens->getCurrValue().value()));
  m_tokens->getNext(); // consume Identifier

  while (TokenType::t_dot == m_tokens->getCurr()) {
    m_tokens->getNext(); // consume `.`
    if (TokenType::t_identifier != m_tokens->getCurr()) {
      // TODO: write error message
      return nullptr;
    }
    parts.push_back(std::get<std::string>(m_tokens->getCurrValue().value()));
    m_tokens->getNext(); // consume identifier
  }

  return std::make_unique<IdentifierNode>(std::move(parts));
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
    TypeNodePtr intType = nullptr;
    switch (m_tokens->getCurr()) {
    case TokenType::t_int8_literal:
      intType = BasicTypeNode::t_int8();
      break;
    case TokenType::t_int16_literal:
      intType = BasicTypeNode::t_int16();
      break;
    case TokenType::t_int32_literal:
      intType = BasicTypeNode::t_int32();
      break;
    case TokenType::t_int64_literal:
      intType = BasicTypeNode::t_int64();
      break;
    default:
      return nullptr;
    }
    int64_t intValue = std::get<int64_t>(m_tokens->getCurrValue().value());
    return std::make_unique<IntLiteralNode>(intValue, std::move(intType));
  } else if (ParserUtilities::isUnsignedIntLiteral(m_tokens->getCurr())) {
    TypeNodePtr uintType = nullptr;
    switch (m_tokens->getCurr()) {
    case TokenType::t_uint8_literal:
      uintType = BasicTypeNode::t_uint8();
      break;
    case TokenType::t_uint16_literal:
      uintType = BasicTypeNode::t_uint16();
      break;
    case TokenType::t_uint32_literal:
      uintType = BasicTypeNode::t_uint32();
      break;
    case TokenType::t_uint64_literal:
      uintType = BasicTypeNode::t_uint64();
      break;
    default:
      return nullptr;
    }
    uint64_t uintValue = std::get<uint64_t>(m_tokens->getCurrValue().value());
    return std::make_unique<UintLiteralNode>(uintValue, std::move(uintType));
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
    auto innerExpr = parseExpression();
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
  auto rhs = parseExpression();
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
  auto expr = parseExpression();
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
      auto param = parseExpression();
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

  auto index = parseExpression();
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

VarDeclPtr Parser::parseVarDecl(Identifier name, bool allowUninitializedConst,
                                bool allowValue) {
  bool isMutable = false;

  if (nullptr == name) {
    if (TokenType::t_var != m_tokens->getCurr()) {
      // TODO: write error message
      return nullptr;
    }
    isMutable = true;

    if (TokenType::t_identifier != m_tokens->getNext()) {
      // TODO: write error message
      return nullptr;
    }
    name = parseIdentifier();
  }

  if (TokenType::t_colon != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }

  TypeNodePtr type = BasicTypeNode::t_implicit();
  if (TokenType::t_equal != m_tokens->getNext() || !allowValue) {
    type = parseType(true);
    if (nullptr == type) {
      // TODO: wrrite error message
      return nullptr;
    }
  }

  if (type->isImplicitType() && !allowValue) { // used for class definition
    // context, no value is allowed but a type has to be specified
    // TODO: wrtie error message
    return nullptr;
  } else if (!allowValue) {
    return std::make_unique<VarDeclNode>(std::move(name), std::move(type),
                                         nullptr);
  }

  Expression expr;
  if (TokenType::t_equal == m_tokens->getCurr()) { // parse initial value
    m_tokens->getNext();                           // consume =

    expr = parseExpression();
    if (nullptr == expr) {
      // TODO: write error message
      return nullptr;
    }
  } else if (!isMutable &&
             !allowUninitializedConst) { // a constant must have a value
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

  return std::make_unique<VarDeclNode>(std::move(name), std::move(type),
                                       std::move(expr));
}

Scope Parser::parseScope(bool forceBrackets, bool disallowBrackets,
                         bool isFileLevel) {
  const bool withBrackets = TokenType::t_open_bracket == m_tokens->getCurr();
  if (disallowBrackets && withBrackets) {
    // TODO: write error message
    return nullptr;
  } else if (forceBrackets && !withBrackets) {
    // TODO: write error message
    return nullptr;
  }

  std::vector<ClassPtr> classes;
  std::vector<FunctionPtr> functions;
  std::vector<Statement> statements;

  ClassPtr classLiteral = nullptr;
  FunctionPtr functionLiteral = nullptr;
  Statement currStatement = nullptr;

  Token openBracket;

  if (withBrackets) {
    openBracket = m_tokens->getCurr();
    m_tokens->getNext(); // consume {
  }

  // for parsing top-level functions
  const bool withName = true;
  do {
    switch (m_tokens->getCurr()) {
    case TokenType::t_close_bracket:
      break;
    case TokenType::t_class:
      classLiteral = parseClass();
      if (nullptr == classLiteral) {
        // TODO: write error message
        return nullptr;
      }
      classes.push_back(std::move(classLiteral));
      break;
    case TokenType::t_func:
      functionLiteral = parseFunction(withName);
      if (nullptr == functionLiteral) {
        // TODO: write error message
        return nullptr;
      }
      functions.push_back(std::move(functionLiteral));
      break;
    default:
      currStatement = parseStatement();
      if (nullptr == currStatement) {
        // TODO: write error message
        return nullptr;
      }
      statements.push_back(std::move(currStatement));
      break;
    }
  } while ((withBrackets || isFileLevel) &&
           TokenType::t_close_bracket != m_tokens->getCurr() &&
           TokenType::t_eof != m_tokens->getCurr());

  if (withBrackets && TokenType::t_close_bracket != m_tokens->getCurr()) {
    ErrorManager::logError(ErrorTypes::E_BAD_TOKEN,
                           {"Expected } but not found", openBracket,
                            m_tokens->getSourceProject()});
    return nullptr;
  }

  if (withBrackets) {
    if (TokenType::t_close_bracket != m_tokens->getCurr()) {
      // TODO: write error message
      return nullptr;
    }
    m_tokens->getNext(); // consume }
  }

  return std::make_unique<ScopeNode>(std::move(classes), std::move(functions),
                                     std::move(statements));
}

Expression Parser::parseConditional() {
  m_tokens->getNext(); // consume if
  Expression cond = parseExpression();
  if (nullptr == cond) {
    // TODO: write error message
    return nullptr;
  }

  Scope body = parseScope();
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
      _else = parseScope();
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
    init = parseExpression();
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
    cond = parseExpression();
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
    advance = parseExpression();
    if (nullptr == advance) {
      // TODO: write error message
      return nullptr;
    }
  }

  auto body = parseScope();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  return std::make_unique<ForLoopNode>(std::move(init), std::move(cond),
                                       std::move(advance), std::move(body));
}

Expression Parser::parseForInLoop() {
  if (TokenType::t_identifier != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  Identifier name = parseIdentifier();

  if (TokenType::t_in != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume in

  Expression iterable = parseExpression();
  if (nullptr == iterable) {
    // TODO: write error message
    return nullptr;
  }

  if (TokenType::t_semicolon == m_tokens->getCurr()) {
    return nullptr;
  }

  Scope body = parseScope();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  return std::make_unique<ForInLoopNode>(std::move(name), std::move(iterable),
                                         std::move(body));
}

Expression Parser::parseWhileLoop() {
  m_tokens->getNext(); // consume while

  Expression cond;
  if (TokenType::t_open_bracket != m_tokens->getCurr()) {
    cond = parseExpression();
    if (nullptr == cond) {
      // TODO: write error message
      return nullptr;
    }
  }

  Scope body = parseScope();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  return std::make_unique<WhileLoopNode>(std::move(cond), std::move(body));
}

Switch Parser::parseSwitch() {
  m_tokens->getNext(); // consume `switch`

  auto expr = parseExpression();
  if (nullptr == expr) {
    // TODO: write error message
    return nullptr;
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

  return std::make_unique<SwitchNode>(std::move(expr), std::move(cases));
}

SwitchCase Parser::parseSwitchCase() {
  auto expr = parseExpression();
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

  auto body = parseScope();
  if (nullptr == body) {
    // TODO: write error message
    return nullptr;
  }

  return std::make_unique<SwitchCaseNode>(std::move(expr), std::move(body));
}

FunctionPtr Parser::parseFunction(bool withName) {
  m_tokens->getNext(); // consume `func`

  Identifier name = nullptr;
  m_tokens->saveAnchor();
  if (withName) {
    if (TokenType::t_identifier != m_tokens->getCurr()) {
      // TODO: write error message
      return nullptr;
    }
    name = parseIdentifier();
  }

  // parse parameters
  std::vector<Parameter> parameters;
  const bool multipleParams = TokenType::t_open_paren == m_tokens->getCurr();

  if (multipleParams) {
    m_tokens->discardAnchor();
    m_tokens->getNext(); // consume (
    TypeNodePtr lastType = nullptr;
    while (TokenType::t_close_paren != m_tokens->getCurr()) {
      if (nullptr != lastType) {
        // if not the first parameter
        if (TokenType::t_comma != m_tokens->getCurr()) {
          ErrorManager::logError(
              ErrorTypes::E_BAD_TOKEN,
              {Formatter("Expected ',', found {}", tokenToString(m_tokens)),
               m_tokens, "Did you forget a comma (',')?"});
          return nullptr;
        }
        m_tokens->getNext(); // consume ,
      }

      // TODO: disable the ErrorManager
      m_tokens->saveAnchorAndCurrentToken();
      TypeNodePtr paramType = parseType();
      if (nullptr == paramType && nullptr == lastType) {
        m_tokens->discardAnchor();
        return nullptr;
      }

      Identifier paramName = nullptr;
      if (TokenType::t_identifier != m_tokens->getCurr()) {
        if (nullptr != lastType && paramType->isSimpleType()) {
          m_tokens->restoreAnchor();
          paramName = parseIdentifier();
          paramType = std::move(lastType);
        } else {
          m_tokens->discardAnchor();
          ErrorManager::logError(ErrorTypes::E_BAD_TOKEN,
                                 {Formatter("Expected identifier, found {}",
                                            tokenToString(m_tokens)),
                                  m_tokens});
          return nullptr;
        }
      } else {
        m_tokens->discardAnchor();
        paramName = parseIdentifier();
      }
      lastType = paramType->clone();
      parameters.push_back(std::make_unique<ParameterNode>(
          std::move(paramType), std::move(paramName)));
    }

    if (TokenType::t_close_paren != m_tokens->getCurr()) {
      ErrorManager::logError(
          ErrorTypes::E_BAD_TOKEN,
          {Formatter("Expected ), found {}", tokenToString(m_tokens)),
           m_tokens});
      return nullptr;
    }
    m_tokens->getNext(); // consume )
  } else if (TokenType::t_arrow != m_tokens->getCurr()) {
    // if there is only one param
    m_tokens->restoreAnchor();
    name = nullptr;
    m_tokens->saveAnchorAndCurrentToken();
    TypeNodePtr paramType = parseType();
    if (nullptr != paramType) {
      m_tokens->discardAnchor();
      if (TokenType::t_identifier != m_tokens->getCurr()) {
        ErrorManager::logError(ErrorTypes::E_BAD_TOKEN,
                               {Formatter("Expected identifier, found {}",
                                          tokenToString(m_tokens)),
                                m_tokens});
        return nullptr;
      }
      Identifier paramName = parseIdentifier();
      parameters.push_back(std::make_unique<ParameterNode>(
          std::move(paramType), std::move(paramName)));
    } else {
      // in case there are no arguments and no parens
      m_tokens->restoreAnchor();
    }
  }

  // parse retType
  bool explicitReturnType = false;
  TypeNodePtr retType = BasicTypeNode::t_void();
  if (TokenType::t_arrow == m_tokens->getCurr()) {
    m_tokens->getNext(); // consume =>
    retType = parseType();
    explicitReturnType = true;
    if (nullptr == retType) {
      return nullptr;
    }
  }

  // parse body
  const bool enforceBrackets = explicitReturnType || !multipleParams;
  if (enforceBrackets && TokenType::t_open_bracket != m_tokens->getCurr()) {
    ErrorManager::logError(
        ErrorTypes::E_BAD_TOKEN,
        {Formatter("A function which {} must enclose its body in brackets",
                   (nullptr != retType) ? ("has an explicit return type")
                                        : ("has no parentheses")),
         m_tokens});
    return nullptr;
  }

  Expression body = parseScope(enforceBrackets);
  if (nullptr == body) {
    return nullptr;
  }

  return std::make_unique<FunctionLiteralNode>(
      std::move(name), std::move(parameters), std::move(retType),
      std::move(body));
}

Import Parser::parseImport() {
  if (TokenType::t_import != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume `import`

  auto name = parseIdentifier();
  if (nullptr == name) {
    // TODO: write error message
    return nullptr;
  }

  return std::make_unique<ImportNode>(std::move(name));
}
}; // namespace rgl
