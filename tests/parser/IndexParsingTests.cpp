#include <memory>

#include "lexer/Token.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/IndexNode.hpp"
#include "parser/ast/expressions/ops/InvokeNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, simpleIndex) {
  auto parser = makeParser({{TokenType::t_identifier, "a"},
                            {TokenType::t_open_square},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_close_square}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<IndexNode>(std::make_unique<IdentifierNode>("a"),
                                  std::make_unique<IdentifierNode>("b")));
}

TEST(Parser, indexWithBinOp) {
  auto parser = makeParser({{TokenType::t_identifier, "a"},
                            {TokenType::t_open_square},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_close_square},
                            {TokenType::t_plus},
                            {TokenType::t_identifier, "c"}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_plus,
          std::make_unique<IndexNode>(std::make_unique<IdentifierNode>("a"),
                                      std::make_unique<IdentifierNode>("b")),
          std::make_unique<IdentifierNode>("c")));
}
