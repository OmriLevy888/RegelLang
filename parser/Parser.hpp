#pragma once
#include "common/Logger.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "lexer/TokenCollection.hpp"
#include "parser/ast/FileNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class Parser : public ILoggable {
public:
  Parser(std::unique_ptr<TokenCollection> &&tokens)
      : m_tokens(std::move(tokens)) {
    m_tokens->getNext(); // get the first token
  }

  std::unique_ptr<FileNode> parseFile();
  Expression parseExprssion();

  std::string toString() const override {
    return Formatter("Parser<{}>", m_tokens->toString());
  }

private:
  std::unique_ptr<TokenCollection> m_tokens;
  std::unique_ptr<FileNode> m_ast;

  uint8_t m_lastPrecedence;

  TypePtr parseType();

  Expression parsePrimary();
  Expression parseRest(Expression primary);

  std::unique_ptr<IdentifierNode> parseIdentifier();
  Expression parseLiteral();
  Expression parseIntLiteral();
  Expression parseRealLiteral();
  Expression parseTextLiteral();
  Expression parseBoolLiteral();

  Expression parseParentheses();
  Expression parseBinOp(Expression primary);
  Expression parsePreOp();
  Expression parsePostOp(Expression expr);

  Expression parseVarDecl();
};
}; // namespace rgl
