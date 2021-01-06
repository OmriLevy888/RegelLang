#include "lexer/Token.hpp"
#include "parser/ast/expressions/ForInLoopNode.hpp"
#include "parser/ast/expressions/ForLoopNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/ops/InvokeNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"
#include "parser/ast/statements/BreakNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

#include "parser/ast/expressions/ConditionalNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"

#include <memory>

using namespace rgl;

TEST(Parser, invokeNoParams) {
  auto parser = makeParser({{TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<InvokeNode>(std::make_unique<IdentifierNode>("foo")));
}

TEST(Parser, invokeOneParam) {
  auto parser = makeParser({{TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<InvokeNode>(std::make_unique<IdentifierNode>("foo"),
                                   std::make_unique<IdentifierNode>("a")));
}

TEST(Parser, invokeMultipleParams) {
  auto parser = makeParser({{TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_close_paren}});

  std::vector<Expression> params;
  params.push_back(std::make_unique<IdentifierNode>("a"));
  params.push_back(std::make_unique<IdentifierNode>("b"));

  assertNode(parser->parseExprssion(),
             std::make_unique<InvokeNode>(
                 std::make_unique<IdentifierNode>("foo"), std::move(params)));
}

TEST(Parser, invokeLastParamWithComma) {
  auto parser = makeParser({{TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_comma},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<InvokeNode>(std::make_unique<IdentifierNode>("foo"),
                                   std::make_unique<IdentifierNode>("a")));
}
