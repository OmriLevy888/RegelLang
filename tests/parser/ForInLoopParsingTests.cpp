#include <memory>

#include "lexer/Token.hpp"
#include "parser/ast/expressions/ConditionalNode.hpp"
#include "parser/ast/expressions/ForInLoopNode.hpp"
#include "parser/ast/expressions/ForLoopNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"
#include "parser/ast/statements/BreakNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, simpleForInLoop) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_identifier, "item"},
                            {TokenType::t_in},
                            {TokenType::t_identifier, "iterable"},
                            {TokenType::t_yield},
                            {TokenType::t_identifier, "item"},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseExpression(),
             std::make_unique<ForInLoopNode>(
                 std::make_unique<IdentifierNode>("item"),
                 std::make_unique<IdentifierNode>("iterable"),
                 std::make_unique<YieldNode>(
                     std::make_unique<IdentifierNode>("item"))));
}

TEST(Parser, consumeForInLoop) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_identifier, "item"},
                            {TokenType::t_in},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "iterable"},
                            {TokenType::t_yield},
                            {TokenType::t_identifier, "item"},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseExpression(),
             std::make_unique<ForInLoopNode>(
                 std::make_unique<IdentifierNode>("item"),
                 std::make_unique<UnaryOpNode>(
                     UnaryOpType::u_pre_colon,
                     std::make_unique<IdentifierNode>("iterable")),
                 std::make_unique<YieldNode>(
                     std::make_unique<IdentifierNode>("item"))));
}

TEST(Parser, mutableForInLoop) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_identifier, "item"},
                            {TokenType::t_in},
                            {TokenType::t_ampersand},
                            {TokenType::t_identifier, "iterable"},
                            {TokenType::t_yield},
                            {TokenType::t_identifier, "item"},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseExpression(),
             std::make_unique<ForInLoopNode>(
                 std::make_unique<IdentifierNode>("item"),
                 std::make_unique<UnaryOpNode>(
                     UnaryOpType::u_pre_ampersand,
                     std::make_unique<IdentifierNode>("iterable")),
                 std::make_unique<YieldNode>(
                     std::make_unique<IdentifierNode>("item"))));
}
