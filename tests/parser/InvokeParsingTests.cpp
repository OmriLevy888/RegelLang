#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/ops/InvokeNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"

#include <memory>

using namespace rgl;

TEST(Parser, invokeNoParams) {
  auto parser = makeParser({{TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<InvokeNode>(std::make_unique<IdentifierNode>("foo"s)));
}

TEST(Parser, invokeOneParam) {
  auto parser = makeParser({{TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<InvokeNode>(std::make_unique<IdentifierNode>("foo"s),
                                   std::make_unique<IdentifierNode>("a"s)));
}

TEST(Parser, invokeMultipleParams) {
  auto parser = makeParser({{TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_close_paren}});

  std::vector<Expression> params;
  params.push_back(std::make_unique<IdentifierNode>("a"s));
  params.push_back(std::make_unique<IdentifierNode>("b"s));

  assertNode(parser->parseExpression(),
             std::make_unique<InvokeNode>(
                 std::make_unique<IdentifierNode>("foo"s), std::move(params)));
}

TEST(Parser, invokeLastParamWithComma) {
  auto parser = makeParser({{TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_comma},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<InvokeNode>(std::make_unique<IdentifierNode>("foo"s),
                                   std::make_unique<IdentifierNode>("a"s)));
}

TEST(Parser, invokeWithBinOp) {
  auto parser = makeParser({{TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_close_paren},
                            {TokenType::t_plus},
                            {TokenType::t_identifier, "b"s}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_plus,
          std::make_unique<InvokeNode>(std::make_unique<IdentifierNode>("foo"s),
                                       std::make_unique<IdentifierNode>("a"s)),
          std::make_unique<IdentifierNode>("b"s)));
}
