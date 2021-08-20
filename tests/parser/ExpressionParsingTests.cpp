#include <memory>

#include "common/errors/ErrorManager.hpp"
#include "common/source-objects/SourceProject.hpp"
#include "common/source-stream/TextSourceStream.hpp"
#include "lexer/DummyTokenGenerator.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "lexer/Token.hpp"
#include "lexer/TokenCollection.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/constructs/BasicTypeNode.hpp"
#include "parser/ast/expressions/ConditionalNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/ScopeNode.hpp"
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
#include "gtest/gtest.h"

using namespace rgl;

TEST(Parser, identifier) {
  auto parser = makeParser(
      {{TokenType::t_identifier, "a"s}, {TokenType::t_identifier, "b"s}});

  assertNode(parser->parseExpression(), std::make_unique<IdentifierNode>("a"s));
  assertNode(parser->parseExpression(), std::make_unique<IdentifierNode>("b"s));
}

TEST(Parser, literal) {
  auto parser = makeParser({{TokenType::t_int16_literal, 15l},
                            {TokenType::t_int32_literal, 30l},
                            {TokenType::t_boolean, true},
                            {TokenType::t_string_literal, "hello"s}});

  assertNode(parser->parseExpression(),
             std::make_unique<IntLiteralNode>(15, BasicTypeNode::t_int16()));
  assertNode(parser->parseExpression(),
             std::make_unique<IntLiteralNode>(30, BasicTypeNode::t_int32()));
  assertNode(parser->parseExpression(),
             std::make_unique<BooleanLiteralNode>(true));
  assertNode(parser->parseExpression(),
             std::make_unique<StringLiteralNode>("hello"));
}

TEST(Parser, precedenceNoParens) {
  auto parser = makeParser({{TokenType::t_int32_literal, 2l},
                            {TokenType::t_asterisk},
                            {TokenType::t_int32_literal, 3l},
                            {TokenType::t_plus},
                            {TokenType::t_int32_literal, 1l}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_plus,
          std::make_unique<BinOpNode>(
              BinOpType::b_asterisk,
              std::make_unique<IntLiteralNode>(2, BasicTypeNode::t_int32()),
              std::make_unique<IntLiteralNode>(3, BasicTypeNode::t_int32())),
          std::make_unique<IntLiteralNode>(1, BasicTypeNode::t_int32())));
}

TEST(Parser, dotPrecedenceNoParens) {
  auto parser = makeParser({{TokenType::t_identifier, "a"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "c"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "d"s},
                            {TokenType::t_plus},
                            {TokenType::t_boolean, true}});

  assertNode(parser->parseExpression(),
             std::make_unique<BinOpNode>(
                 BinOpType::b_plus,
                 std::make_unique<IdentifierNode>(
                     std::vector<std::string>{"a", "b", "c", "d"}),
                 std::make_unique<BooleanLiteralNode>(true)));
}

TEST(Parser, precedenceMultipleParts) {
  auto parser = makeParser({{TokenType::t_identifier, "a"s},
                            {TokenType::t_asterisk},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_plus},
                            {TokenType::t_identifier, "c"s},
                            {TokenType::t_asterisk},
                            {TokenType::t_identifier, "d"s}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_plus,
          std::make_unique<BinOpNode>(BinOpType::b_asterisk,
                                      std::make_unique<IdentifierNode>("a"s),
                                      std::make_unique<IdentifierNode>("b"s)),
          std::make_unique<BinOpNode>(BinOpType::b_asterisk,
                                      std::make_unique<IdentifierNode>("c"s),
                                      std::make_unique<IdentifierNode>("d"s))));
}

TEST(Parser, precedenceWithParens) {
  auto parser = makeParser({{TokenType::t_int32_literal, 2l},
                            {TokenType::t_asterisk},
                            {TokenType::t_open_paren},
                            {TokenType::t_int32_literal, 3l},
                            {TokenType::t_plus},
                            {TokenType::t_int32_literal, 1l},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_asterisk,
          std::make_unique<IntLiteralNode>(2, BasicTypeNode::t_int32()),
          std::make_unique<ParenthesesNode>(std::make_unique<BinOpNode>(
              BinOpType::b_plus,
              std::make_unique<IntLiteralNode>(3, BasicTypeNode::t_int32()),
              std::make_unique<IntLiteralNode>(1, BasicTypeNode::t_int32())))));
}

TEST(Parser, selfModifyingBinOp) {
  auto parser = makeParser({{TokenType::t_identifier, "a"s},
                            {TokenType::t_percent_equal},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_minus},
                            {TokenType::t_identifier, "c"s}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_percent_equal, std::make_unique<IdentifierNode>("a"s),
          std::make_unique<BinOpNode>(BinOpType::b_minus,
                                      std::make_unique<IdentifierNode>("b"s),
                                      std::make_unique<IdentifierNode>("c"s))));
}

TEST(Parser, varNoValue) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "i32"s}});

  assertNode(parser->parseExpression(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"s),
                 BasicTypeNode::t_int32()->getOwningType(), nullptr));
}

TEST(Parser, varImplicitTypeWithValue) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_colon},
                            {TokenType::t_equal},
                            {TokenType::t_boolean, true}});

  assertNode(parser->parseExpression(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"s),
                 BasicTypeNode::t_implicit()->getMutableType(),
                 std::make_unique<BooleanLiteralNode>(true)));
}

TEST(Parser, letExplicitTypeWithValue) {
  auto parser = makeParser({{TokenType::t_identifier, "a"s},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "bar"s},
                            {TokenType::t_equal},
                            {TokenType::t_int32_literal, 10l}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<VarDeclNode>(
          std::make_unique<IdentifierNode>("a"s),
          std::make_unique<BasicTypeNode>(
              std::vector<std::string>{"foo", "bar"}, TypeProperties::_owning),
          std::make_unique<IntLiteralNode>(10, BasicTypeNode::t_int32())));
}

TEST(Parser, emptyBlock) {
  auto parser = makeParser({{TokenType::t_identifier, "a"s},
                            {TokenType::t_colon},
                            {TokenType::t_equal},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"s),
                 BasicTypeNode::t_implicit()->getOwningType(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, fullBlock) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_equal},
                            {TokenType::t_int32_literal, 1l},
                            {TokenType::t_plus},
                            {TokenType::t_open_bracket},
                            {TokenType::t_yield},
                            {TokenType::t_int32_literal, 2l},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  std::vector<Statement> statements;
  statements.push_back(std::make_unique<YieldNode>(
      std::make_unique<IntLiteralNode>(2, BasicTypeNode::t_int32())));

  assertNode(
      parser->parseExpression(),
      std::make_unique<VarDeclNode>(
          std::make_unique<IdentifierNode>("a"s),
          std::make_unique<BasicTypeNode>(std::vector<std::string>{"b"},
                                          BitField(TypeProperties::_mutable) |
                                              TypeProperties::_owning),
          std::make_unique<BinOpNode>(
              BinOpType::b_plus,
              std::make_unique<IntLiteralNode>(1, BasicTypeNode::t_int32()),
              std::make_unique<ScopeNode>(std::move(statements)))));
}

TEST(Parser, compoundTypeNoIdentifier) {
  auto parser = makeParser("TEST::Parser.compoundTypeNoIdentifier",
                           {{{TokenType::t_identifier, {0, 1, 0, 1}}, "a"s},
                            {{TokenType::t_colon, {0, 1, 2, 1}}},
                            {{TokenType::t_identifier, {0, 1, 4, 3}}, "foo"s},
                            {{TokenType::t_dot, {0, 1, 7, 1}}},
                            {{TokenType::t_equal, {0, 1, 9, 1}}}},
                           {"b := 5;", "a : foo. = 1;", "return a + b;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, parensMissingClose) {
  auto parser = makeParser("TEST::Parser.parensMissingClose",
                           {{{TokenType::t_identifier, {0, 0, 0, 1}}, "a"s},
                            {{TokenType::t_asterisk, {0, 0, 2, 1}}},
                            {{TokenType::t_open_paren, {0, 0, 4, 1}}},
                            {{TokenType::t_identifier, {0, 0, 5, 1}}, "b"s},
                            {{TokenType::t_plus, {0, 0, 7, 1}}},
                            {{TokenType::t_identifier, {0, 0, 9, 1}}, "c"s},
                            {{TokenType::t_semicolon, {0, 0, 10, 1}}}},
                           {"a * (b + c;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, invokeMissingClose) {
  auto parser = makeParser("TEST::Parser.invokeMissingClose",
                           {{{TokenType::t_identifier, {0, 0, 0, 1}}, "a"s},
                            {{TokenType::t_open_paren, {0, 0, 1, 1}}},
                            {{TokenType::t_identifier, {0, 1, 0, 1}}, "b"s},
                            {{TokenType::t_comma, {0, 1, 1, 1}}},
                            {{TokenType::t_identifier, {0, 1, 3, 1}}, "c"s},
                            {{TokenType::t_semicolon, {0, 1, 4, 1}}}},
                           {"a(", "b, c;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, indexMissingClose) {
  auto parser = makeParser("TEST::Parser.indexMissingClose",
                           {{{TokenType::t_identifier, {0, 0, 0, 1}}, "a"s},
                            {{TokenType::t_open_square, {0, 0, 1, 1}}},
                            {{TokenType::t_semicolon, {0, 0, 2, 1}}}},
                           {"a[;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, blockMissingClose) {
  auto parser = makeParser("TEST::Parser.blockMissingClose",
                           {{{TokenType::t_open_bracket, {0, 0, 0, 1}}},
                            {{TokenType::t_yield, {0, 1, 4, 5}}},
                            {{TokenType::t_identifier, {0, 1, 10, 1}}, "a"s},
                            {{TokenType::t_semicolon, {0, 1, 11, 1}}}},
                           {"{", "    yield a;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}
