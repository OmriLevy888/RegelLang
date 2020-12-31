#include "lexer/DummyTokenGenerator.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/statement/BreakNode.hpp"
#include "parser/ast/statement/ContinueNode.hpp"
#include "parser/ast/statement/ReturnNode.hpp"
#include "parser/ast/statement/YieldNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"
#include <memory>

using namespace rgl;

TEST(Parser, keywordStatement) {
  auto parser = makeParser({{TokenType::t_continue},
                            {TokenType::t_semicolon},
                            {TokenType::t_break},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseStatement(), std::make_unique<ContinueNode>());
  assertNode(parser->parseStatement(), std::make_unique<BreakNode>());
}

TEST(Parser, simpleStatement) {
  auto parser = makeParser({{TokenType::t_yield},
                            {TokenType::t_int32_literal, 5},
                            {TokenType::t_plus},
                            {TokenType::t_int32_literal, 5},
                            {TokenType::t_semicolon},
                            {TokenType::t_return},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseStatement(),
             std::make_unique<YieldNode>(std::make_unique<BinOpNode>(
                 BinOpType::b_plus,
                 std::make_unique<IntLiteralNode>(5, Type::t_int32()),
                 std::make_unique<IntLiteralNode>(5, Type::t_int32()))));
  assertNode(parser->parseStatement(), std::make_unique<ReturnNode>());
}
