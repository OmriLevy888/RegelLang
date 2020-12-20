#pragma once
#include "common/Logger.hpp"
#include "lexer/TokenCollection.hpp"
#include "parser/ast/FileNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteral.hpp"
#include "parser/ast/expressions/literals/CharLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/RealLiteralNode.hpp"
#include "parser/ast/expressions/literals/StringLiteralNode.hpp"

namespace rgl {
using Expression = std::unique_ptr<ExpressionNode>;
class Parser : public ILoggable {
public:
  Parser();

  std::unique_ptr<FileNode> parseFile();

private:
  std::unique_ptr<TokenCollection> m_tokens;
  std::unique_ptr<FileNode> m_ast;

  Expression parseExprssion();

  Expression parseIdentifier();
  Expression parseLiteral();
  Expression parseIntLiteral();
  Expression parseRealLiteral();
  Expression parseTextLiteral();
  Expression parseBoolLiteral();
};
}; // namespace rgl
