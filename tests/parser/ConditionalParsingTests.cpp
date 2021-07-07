#include <memory>

#include "lexer/Token.hpp"
#include "parser/ast/expressions/BasicIdentifierNode.hpp"
#include "parser/ast/expressions/ConditionalNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/statements/BreakNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/ReturnNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, simpleIf) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_open_bracket},
                            {TokenType::t_boolean, true},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<ConditionalNode>(
                 std::make_unique<BooleanLiteralNode>(false),
                 std::make_unique<ExpressionStatementNode>(
                     std::make_unique<BooleanLiteralNode>(true))));
}

TEST(Parser, sinlgeStatementIf) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_int32_literal, 5l},
                            {TokenType::t_int8_literal, 1l},
                            {TokenType::t_semicolon}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ConditionalNode>(
          std::make_unique<IntLiteralNode>(5, BasicType::t_int32()),
          std::make_unique<ExpressionStatementNode>(
              std::make_unique<IntLiteralNode>(1, BasicType::t_int8()))));
}

TEST(Parser, elifNoElse) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_boolean, true},
                            {TokenType::t_boolean, false},
                            {TokenType::t_semicolon},
                            {TokenType::t_elif},
                            {TokenType::t_boolean, true},
                            {TokenType::t_open_bracket},
                            {TokenType::t_yield},
                            {TokenType::t_int32_literal, 0l},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ConditionalNode>(
          std::make_unique<BooleanLiteralNode>(true),
          std::make_unique<ExpressionStatementNode>(
              std::make_unique<BooleanLiteralNode>(false)),
          std::make_unique<ConditionalNode>(
              std::make_unique<BooleanLiteralNode>(true),
              std::make_unique<YieldNode>(
                  std::make_unique<IntLiteralNode>(0, BasicType::t_int32())))));
}

TEST(Parser, elseNoElif) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_boolean, true},
                            {TokenType::t_open_bracket},
                            {TokenType::t_yield},
                            {TokenType::t_boolean, true},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket},
                            {TokenType::t_else},
                            {TokenType::t_int32_literal, 0l},
                            {TokenType::t_semicolon}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ConditionalNode>(
          std::make_unique<BooleanLiteralNode>(true),
          std::make_unique<YieldNode>(
              std::make_unique<BooleanLiteralNode>(true)),
          std::make_unique<ExpressionStatementNode>(
              std::make_unique<IntLiteralNode>(0, BasicType::t_int32()))));
}

TEST(Parser, multipleElifsAndElse) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_int32_literal, 0l},
                            {TokenType::t_semicolon},
                            {TokenType::t_elif},
                            {TokenType::t_boolean, false},
                            {TokenType::t_int32_literal, 1l},
                            {TokenType::t_semicolon},
                            {TokenType::t_elif},
                            {TokenType::t_boolean, true},
                            {TokenType::t_int32_literal, 2l},
                            {TokenType::t_semicolon},
                            {TokenType::t_else},
                            {TokenType::t_int32_literal, 3l},
                            {TokenType::t_semicolon}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ConditionalNode>(
          std::make_unique<BooleanLiteralNode>(false),
          std::make_unique<ExpressionStatementNode>(
              std::make_unique<IntLiteralNode>(0, BasicType::t_int32())),
          std::make_unique<ConditionalNode>(
              std::make_unique<BooleanLiteralNode>(false),
              std::make_unique<ExpressionStatementNode>(
                  std::make_unique<IntLiteralNode>(1, BasicType::t_int32())),
              std::make_unique<ConditionalNode>(
                  std::make_unique<BooleanLiteralNode>(true),
                  std::make_unique<ExpressionStatementNode>(
                      std::make_unique<IntLiteralNode>(2,
                                                       BasicType::t_int32())),
                  std::make_unique<ExpressionStatementNode>(
                      std::make_unique<IntLiteralNode>(
                          3, BasicType::t_int32()))))));
}

TEST(Parser, compoundElif) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_int32_literal, 0l},
                            {TokenType::t_semicolon},
                            {TokenType::t_elif},
                            {TokenType::t_boolean, true},
                            {TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_int32_literal, 1l},
                            {TokenType::t_semicolon},
                            {TokenType::t_elif},
                            {TokenType::t_boolean, true},
                            {TokenType::t_int32_literal, 2l},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseExpression(),
             std::make_unique<ConditionalNode>(
                 std::make_unique<BooleanLiteralNode>(false),
                 std::make_unique<ExpressionStatementNode>(
                     std::make_unique<IntLiteralNode>(0, BasicType::t_int32())),
                 std::make_unique<ConditionalNode>(
                     std::make_unique<BooleanLiteralNode>(true),
                     std::make_unique<ExpressionStatementNode>(
                         std::make_unique<ConditionalNode>(
                             std::make_unique<BooleanLiteralNode>(false),
                             std::make_unique<ExpressionStatementNode>(
                                 std::make_unique<IntLiteralNode>(
                                     1, BasicType::t_int32())),
                             std::make_unique<ConditionalNode>(
                                 std::make_unique<BooleanLiteralNode>(true),
                                 std::make_unique<ExpressionStatementNode>(
                                     std::make_unique<IntLiteralNode>(
                                         2, BasicType::t_int32()))))))));
}

TEST(Parser, compoundElse) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_return},
                            {TokenType::t_semicolon},

                            {TokenType::t_else},
                            {TokenType::t_open_bracket},

                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_equal},
                            {TokenType::t_int32_literal, 10l},
                            {TokenType::t_semicolon},

                            {TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_return},
                            {TokenType::t_semicolon},

                            {TokenType::t_else},
                            {TokenType::t_break},
                            {TokenType::t_semicolon},

                            {TokenType::t_close_bracket}});

  std::vector<Statement> elseStatements;
  elseStatements.push_back(
      std::make_unique<ExpressionStatementNode>(std::make_unique<BinOpNode>(
          BinOpType::b_equal, std::make_unique<BasicIdentifierNode>("a"),
          std::make_unique<IntLiteralNode>(10, BasicType::t_int32()))));

  elseStatements.push_back(std::make_unique<ExpressionStatementNode>(
      std::make_unique<ConditionalNode>(
          std::make_unique<BooleanLiteralNode>(false),
          std::make_unique<ReturnNode>(), std::make_unique<BreakNode>())));

  assertNode(parser->parseExpression(),
             std::make_unique<ConditionalNode>(
                 std::make_unique<BooleanLiteralNode>(false),
                 std::make_unique<ReturnNode>(),
                 std::make_unique<ScopeNode>(std::move(elseStatements))));
}
