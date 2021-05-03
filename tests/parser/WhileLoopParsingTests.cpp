#include "lexer/Token.hpp"
#include "parser/ast/expressions/WhileLoopNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

#include <memory>

using namespace rgl;

TEST(Parser, simpleWhileLoop) {
  auto parser = makeParser({{TokenType::t_while},
                            {TokenType::t_boolean, true},
                            {TokenType::t_yield},
                            {TokenType::t_int32_literal, 1l},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseExpression(),
             std::make_unique<WhileLoopNode>(
                 std::make_unique<BooleanLiteralNode>(true),
                 std::make_unique<YieldNode>(
                     std::make_unique<IntLiteralNode>(1, BasicType::t_int32()))));
}

TEST(Parser, whileLoopNoCond) {
  auto parser = makeParser({{TokenType::t_while},
                            {TokenType::t_open_bracket},
                            {TokenType::t_yield},
                            {TokenType::t_int32_literal, 1l},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<WhileLoopNode>(
          nullptr, std::make_unique<YieldNode>(

                       std::make_unique<IntLiteralNode>(1, BasicType::t_int32()))));
}

TEST(Parser, whileLoopNoBody) {
  auto parser = makeParser({{TokenType::t_while},
                            {TokenType::t_boolean, true},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseExpression(),
             std::make_unique<WhileLoopNode>(
                 std::make_unique<BooleanLiteralNode>(true),
                 std::make_unique<ExpressionStatementNode>()));
}
