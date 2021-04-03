#include <memory>

#include "common/collections/source-objects/SourceFile.hpp"
#include "common/collections/source-objects/SourceLine.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
#include "common/errors/ErrorManager.hpp"
#include "gtest/gtest.h"
#include "lexer/DummyTokenGenerator.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "lexer/Token.hpp"
#include "lexer/TokenCollection.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/constructs/Type.hpp"
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/ConditionalNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/StringLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/ParenthesesNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, identifier) {
  auto parser = makeParser(
      {{TokenType::t_identifier, "a"}, {TokenType::t_identifier, "b"}});

  assertNode(parser->parseExpression(), std::make_unique<IdentifierNode>("a"));
  assertNode(parser->parseExpression(), std::make_unique<IdentifierNode>("b"));
}

TEST(Parser, literal) {
  auto parser = makeParser({{TokenType::t_int16_literal, {15}},
                            {TokenType::t_int32_literal, {30}},
                            {TokenType::t_boolean, {true}},
                            {TokenType::t_string_literal, {"hello"}}});

  assertNode(parser->parseExpression(),
             std::make_unique<IntLiteralNode>(15, BasicType::t_int16()));
  assertNode(parser->parseExpression(),
             std::make_unique<IntLiteralNode>(30, BasicType::t_int32()));
  assertNode(parser->parseExpression(),
             std::make_unique<BooleanLiteralNode>(true));
  assertNode(parser->parseExpression(),
             std::make_unique<StringLiteralNode>("hello"));
}

TEST(Parser, precedenceNoParens) {
  auto parser = makeParser({{TokenType::t_int32_literal, {2}},
                            {TokenType::t_asterisk},
                            {TokenType::t_int32_literal, {3}},
                            {TokenType::t_plus},
                            {TokenType::t_int32_literal, {1}}});

  assertNode(parser->parseExpression(),
             std::make_unique<BinOpNode>(
                 BinOpType::b_plus,
                 std::make_unique<BinOpNode>(
                     BinOpType::b_asterisk,
                     std::make_unique<IntLiteralNode>(2, BasicType::t_int32()),
                     std::make_unique<IntLiteralNode>(3, BasicType::t_int32())),
                 std::make_unique<IntLiteralNode>(1, BasicType::t_int32())));
}

TEST(Parser, dotPrecedenceNoParens) {
  auto parser = makeParser({{TokenType::t_identifier, {"a"}},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, {"b"}},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, {"c"}},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, {"d"}},
                            {TokenType::t_plus},
                            {TokenType::t_boolean, {true}}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_plus,
          std::make_unique<BinOpNode>(
              BinOpType::b_dot,
              std::make_unique<BinOpNode>(
                  BinOpType::b_dot,
                  std::make_unique<BinOpNode>(
                      BinOpType::b_dot, std::make_unique<IdentifierNode>("a"),
                      std::make_unique<IdentifierNode>("b")),
                  std::make_unique<IdentifierNode>("c")),
              std::make_unique<IdentifierNode>("d")),
          std::make_unique<BooleanLiteralNode>(true)));
}

TEST(Parser, precedenceMultipleParts) {
  auto parser = makeParser({{TokenType::t_identifier, "a"},
                            {TokenType::t_asterisk},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_plus},
                            {TokenType::t_identifier, "c"},
                            {TokenType::t_asterisk},
                            {TokenType::t_identifier, "d"}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_plus,
          std::make_unique<BinOpNode>(BinOpType::b_asterisk,
                                      std::make_unique<IdentifierNode>("a"),
                                      std::make_unique<IdentifierNode>("b")),
          std::make_unique<BinOpNode>(BinOpType::b_asterisk,
                                      std::make_unique<IdentifierNode>("c"),
                                      std::make_unique<IdentifierNode>("d"))));
}

TEST(Parser, precedenceWithParens) {
  auto parser = makeParser({{TokenType::t_int32_literal, {2}},
                            {TokenType::t_asterisk},
                            {TokenType::t_open_paren},
                            {TokenType::t_int32_literal, {3}},
                            {TokenType::t_plus},
                            {TokenType::t_int32_literal, {1}},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_asterisk,
          std::make_unique<IntLiteralNode>(2, BasicType::t_int32()),
          std::make_unique<ParenthesesNode>(std::make_unique<BinOpNode>(
              BinOpType::b_plus,
              std::make_unique<IntLiteralNode>(3, BasicType::t_int32()),
              std::make_unique<IntLiteralNode>(1, BasicType::t_int32())))));
}

TEST(Parser, selfModifyingBinOp) {
  auto parser = makeParser({{TokenType::t_identifier, "a"},
                            {TokenType::t_percent_equal},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_minus},
                            {TokenType::t_identifier, "c"}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_percent_equal, std::make_unique<IdentifierNode>("a"),
          std::make_unique<BinOpNode>(BinOpType::b_minus,
                                      std::make_unique<IdentifierNode>("b"),
                                      std::make_unique<IdentifierNode>("c"))));
}

TEST(Parser, varNoValue) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "i32"}});

  assertNode(parser->parseExpression(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 BasicType::t_int32()->getOwningType(), nullptr));
}

TEST(Parser, varImplicitTypeWithValue) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_equal},
                            {TokenType::t_boolean, true}});

  assertNode(parser->parseExpression(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 BasicType::t_implicit()->getMutableType(),
                 std::make_unique<BooleanLiteralNode>(true)));
}

TEST(Parser, letExplicitTypeWithValue) {
  auto parser = makeParser({{TokenType::t_let},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "bar"},
                            {TokenType::t_equal},
                            {TokenType::t_int32_literal, 10}});

  assertNode(parser->parseExpression(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 BasicType::make({"foo", "bar"}, TypeProperties::_owning),
                 std::make_unique<IntLiteralNode>(10, BasicType::t_int32())));
}

TEST(Parser, emptyBlock) {
  auto parser = makeParser({{TokenType::t_let},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_equal},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<VarDeclNode>(std::make_unique<IdentifierNode>("a"),
                                    BasicType::t_implicit()->getOwningType(),
                                    std::make_unique<BlockNode>()));
}

TEST(Parser, fullBlock) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_equal},
                            {TokenType::t_int32_literal, 1},
                            {TokenType::t_plus},
                            {TokenType::t_open_bracket},
                            {TokenType::t_yield},
                            {TokenType::t_int32_literal, 2},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  std::vector<Statement> statements;
  statements.push_back(std::make_unique<YieldNode>(
      std::make_unique<IntLiteralNode>(2, BasicType::t_int32())));

  assertNode(parser->parseExpression(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 BasicType::make({"b"}, BitField(TypeProperties::_mutable) |
                                            TypeProperties::_owning),
                 std::make_unique<BinOpNode>(
                     BinOpType::b_plus,
                     std::make_unique<IntLiteralNode>(1, BasicType::t_int32()),
                     std::make_unique<BlockNode>(std::move(statements)))));
}

TEST(Parser, typeNoIdentifier) {
  auto parser = makeParser("TEST::Parser.typeNoIdentifier",
                           {{{0, TokenType::t_let, 0, 3, 1}},
                            {{1, TokenType::t_identifier, 4, 1, 1}, "a"},
                            {{2, TokenType::t_colon, 9, 1, 1}},
                            {{3, TokenType::t_equal, 11, 1, 1}}},
                           {"let b = 5;", "let a    : = 1;", "return a + b;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, compoundTypeNoIdentifier) {
  auto parser =
      makeParser("TEST::Parser.compoundTypeNoIdentifier",
                 {{{0, TokenType::t_let, 0, 3, 1}},
                  {{1, TokenType::t_identifier, 4, 1, 1}, "a"},
                  {{2, TokenType::t_colon, 6, 1, 1}},
                  {{3, TokenType::t_identifier, 8, 3, 1}, "foo"},
                  {{4, TokenType::t_dot, 11, 1, 1}},
                  {{5, TokenType::t_equal, 13, 1, 1}}},
                 {"let b = 5;", "let a : foo. = 1;", "return a + b;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, parensMissingClose) {
  auto parser = makeParser("TEST::Parser.parensMissingClose",
                           {{{0, TokenType::t_identifier, 0, 1}, "a"},
                            {{1, TokenType::t_asterisk, 2, 1}},
                            {{2, TokenType::t_open_paren, 4, 1}},
                            {{3, TokenType::t_identifier, 5, 1}, "b"},
                            {{4, TokenType::t_plus, 7, 1}},
                            {{5, TokenType::t_identifier, 9, 1}, "c"},
                            {{6, TokenType::t_semicolon, 10, 1}}},
                           {"a * (b + c;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, invokeMissingClose) {
  auto parser = makeParser("TEST::Parser.invokeMissingClose",
                           {{{0, TokenType::t_identifier, 0, 1, 0}, "a"},
                            {{1, TokenType::t_open_paren, 1, 1, 0}},
                            {{0, TokenType::t_identifier, 0, 1, 1}, "b"},
                            {{1, TokenType::t_comma, 1, 1, 1}},
                            {{2, TokenType::t_identifier, 3, 1, 1}, "c"},
                            {{3, TokenType::t_semicolon, 4, 1, 1}}},
                           {"a(", "b, c;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, indexMissingClose) {
  auto parser = makeParser("TEST::Parser.indexMissingClose",
                           {{{0, TokenType::t_identifier, 0, 1}, "a"},
                            {{1, TokenType::t_open_square, 1, 1}},
                            {{2, TokenType::t_semicolon, 2, 1}}},
                           {"a[;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, blockMissingClose) {
  auto parser = makeParser("TEST::Parser.blockMissingClose",
                           {{{0, TokenType::t_open_bracket, 0, 1, 0}},
                            {{0, TokenType::t_yield, 4, 5, 1}},
                            {{1, TokenType::t_identifier, 10, 1, 1}, "a"},
                            {{2, TokenType::t_semicolon, 11, 1, 1}}},
                           {"{", "    yield a;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}
