#pragma once
#include "common/Logger.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "lexer/TokenCollection.hpp"
#include "parser/ast/constructs/FileNode.hpp"
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/SwitchNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"

namespace rgl {
class Parser : public ILoggable {
public:
  Parser(std::unique_ptr<TokenCollection> &&tokens)
      : m_tokens(std::move(tokens)) {
    m_tokens->getNext(); // get the first token
  }

  File parseFile();
  Expression parseExpression();
  Statement parseStatement();
  TypePtr parseType(bool skipQualifiers = false);

  std::string toString() const override {
    return Formatter("Parser<{}>", m_tokens->toString());
  }

private:
  std::unique_ptr<TokenCollection> m_tokens;
  uint8_t m_lastPrecedence;

  std::optional<BitField<TypeProperties>> parseTypeModifiers();
  TypePtr parseBasicType(BitField<TypeProperties> properties);
  TypePtr parseFunctionType(BitField<TypeProperties> properties);

  Expression parsePrimary();
  Expression parseRest(Expression primary);
  Expression parseImplicitStatementExpression();

  Identifier parseIdentifier();
  Expression parseLiteral();
  Expression parseIntLiteral();
  Expression parseRealLiteral();
  Expression parseTextLiteral();
  Expression parseBoolLiteral();

  Expression parseParentheses();
  Expression parseBinOp(Expression primary);
  Expression parsePreOp();
  Expression parsePostOp(Expression primary);
  Expression parseInvoke(Expression primary);
  Expression parseIndex(Expression primary);

  Expression parseVarDecl(Identifier name = nullptr,
                          bool allowUninitializedConst = false);
  Block parseBlock(bool forceBrackets = false);
  Expression parseConditional();
  Expression parseForLoop();
  Expression parseForInLoop();
  Expression parseWhileLoop();

  Switch parseSwitch();
  SwitchCase parseSwitchCase();

  Statement parseKeywordStatement();
  Statement parseSimpleStatement();

  Expression parseFunction();
};
}; // namespace rgl
