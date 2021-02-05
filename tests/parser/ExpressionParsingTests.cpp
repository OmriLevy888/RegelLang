#include "common/collections/source-objects/SourceFile.hpp"
#include "common/collections/source-objects/SourceLine.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
#include "common/errors/ErrorManager.hpp"
#include "lexer/DummyTokenGenerator.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "lexer/Token.hpp"
#include "lexer/TokenCollection.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/Type.hpp"
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/ConditionalNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/StringLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/ParenthesesNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"
#include "tests/TestsCore.hpp"

#include "tests/parser/ParserTestsUtilities.hpp"

#include "gtest/gtest.h"
#include <memory>

using namespace rgl;

TEST(Parser, identifier) {
  auto parser = makeParser(
      {{TokenType::t_identifier, "a"}, {TokenType::t_identifier, "b"}});

  assertNode(parser->parseExprssion(), std::make_unique<IdentifierNode>("a"));
  assertNode(parser->parseExprssion(), std::make_unique<IdentifierNode>("b"));
}

TEST(Parser, literal) {
  auto parser = makeParser({{TokenType::t_int16_literal, {15}},
                            {TokenType::t_int32_literal, {30}},
                            {TokenType::t_boolean, {true}},
                            {TokenType::t_string_literal, {"hello"}}});

  assertNode(parser->parseExprssion(),
             std::make_unique<IntLiteralNode>(15, Type::t_int16()));
  assertNode(parser->parseExprssion(),
             std::make_unique<IntLiteralNode>(30, Type::t_int32()));
  assertNode(parser->parseExprssion(),
             std::make_unique<BooleanLiteralNode>(true));
  assertNode(parser->parseExprssion(),
             std::make_unique<StringLiteralNode>("hello"));
}

TEST(Parser, precedenceNoParens) {
  auto parser = makeParser({{TokenType::t_int32_literal, {2}},
                            {TokenType::t_asterisk},
                            {TokenType::t_int32_literal, {3}},
                            {TokenType::t_plus},
                            {TokenType::t_int32_literal, {1}}});

  assertNode(parser->parseExprssion(),
             std::make_unique<BinOpNode>(
                 BinOpType::b_plus,
                 std::make_unique<BinOpNode>(
                     BinOpType::b_asterisk,
                     std::make_unique<IntLiteralNode>(2, Type::t_int32()),
                     std::make_unique<IntLiteralNode>(3, Type::t_int32())),
                 std::make_unique<IntLiteralNode>(1, Type::t_int32())));
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
      parser->parseExprssion(),
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
      parser->parseExprssion(),
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

  assertNode(parser->parseExprssion(),
             std::make_unique<BinOpNode>(
                 BinOpType::b_asterisk,
                 std::make_unique<IntLiteralNode>(2, Type::t_int32()),
                 std::make_unique<ParenthesesNode>(std::make_unique<BinOpNode>(
                     BinOpType::b_plus,
                     std::make_unique<IntLiteralNode>(3, Type::t_int32()),
                     std::make_unique<IntLiteralNode>(1, Type::t_int32())))));
}

TEST(Parser, selfModifyingBinOp) {
  auto parser = makeParser({{TokenType::t_identifier, "a"},
                            {TokenType::t_percent_equal},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_minus},
                            {TokenType::t_identifier, "c"}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<BinOpNode>(
          BinOpType::b_percent_equal, std::make_unique<IdentifierNode>("a"),
          std::make_unique<BinOpNode>(BinOpType::b_minus,
                                      std::make_unique<IdentifierNode>("b"),
                                      std::make_unique<IdentifierNode>("c"))));
}

TEST(Parser, preUnaryOp) {
  auto parser = makeParser({{TokenType::t_plus_plus},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_asterisk},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_plus},
                            {TokenType::t_exclamation},
                            {TokenType::t_identifier, "c"},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<BinOpNode>(
          BinOpType::b_asterisk,
          std::make_unique<UnaryOpNode>(UnaryOpType::u_pre_plus_plus,
                                        std::make_unique<IdentifierNode>("a")),
          std::make_unique<ParenthesesNode>(std::make_unique<BinOpNode>(
              BinOpType::b_plus, std::make_unique<IdentifierNode>("b"),
              std::make_unique<UnaryOpNode>(
                  UnaryOpType::u_pre_exclamation,
                  std::make_unique<IdentifierNode>("c"))))));
}

TEST(Parser, postUnaryOp) {
  auto parser = makeParser({{TokenType::t_identifier, "a"},
                            {TokenType::t_minus_minus},
                            {TokenType::t_forward_slash},
                            {TokenType::t_identifier, "b"}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<BinOpNode>(
          BinOpType::b_forward_slash,
          std::make_unique<UnaryOpNode>(UnaryOpType::u_post_minus_minus,
                                        std::make_unique<IdentifierNode>("a")),
          std::make_unique<IdentifierNode>("b")));
}

TEST(Parser, varNoValue) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "i32"}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<VarDeclNode>(std::make_unique<IdentifierNode>("a"),
                                    Type::t_int32()->getOwningType(), nullptr));
}

TEST(Parser, varImplicitTypeWithValue) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_equal},
                            {TokenType::t_boolean, true}});

  assertNode(parser->parseExprssion(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 Type::t_implicit()->getMutableType(),
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

  assertNode(parser->parseExprssion(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 makeType({"foo", "bar"}, TypeProperties::_owning),
                 std::make_unique<IntLiteralNode>(10, Type::t_int32())));
}

TEST(Parser, emptyBlock) {
  auto parser = makeParser({{TokenType::t_let},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_equal},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<VarDeclNode>(std::make_unique<IdentifierNode>("a"),
                                    Type::t_implicit()->getOwningType(),
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
      std::make_unique<IntLiteralNode>(2, Type::t_int32())));

  assertNode(parser->parseExprssion(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 makeType({"b"}, BitField(TypeProperties::_mutable) |
                                     TypeProperties::_owning),
                 std::make_unique<BinOpNode>(
                     BinOpType::b_plus,
                     std::make_unique<IntLiteralNode>(1, Type::t_int32()),
                     std::make_unique<BlockNode>(std::move(statements)))));
}

TEST(Parser, typeNoIdentifier) {
  std::vector<TokenValuePair> tokens{
      {{0, TokenType::t_let, 0, 3, 1}},
      {{1, TokenType::t_identifier, 4, 1, 1}, "a"},
      {{2, TokenType::t_colon, 9, 1, 1}},
      {{3, TokenType::t_equal, 11, 1, 1}}};
  auto project =
      std::make_shared<SourceProject>("TEST::Parser.typeNoIdentifier");
  SourceFile file{"TEST::Parser.typeNoIdentifier"};
  file.m_lines.push_back(SourceLine("let b = 5;"));
  file.m_lines.push_back(SourceLine("let a    : = 1;", tokens));
  file.m_lines.push_back(SourceLine("return a + b;"));
  project->addFile(std::move(file));
  auto parser = makeParser(std::move(tokens), project);

  ASSERT_EQ(parser->parseExprssion(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

#include <iostream>

TEST(Parser, compoundTypeNoIdentifier) {
  std::vector<TokenValuePair> tokens{
      {{0, TokenType::t_let, 0, 3, 1}},
      {{1, TokenType::t_identifier, 4, 1, 1}, "a"},
      {{2, TokenType::t_colon, 6, 1, 1}},
      {{3, TokenType::t_identifier, 8, 3, 1}, "foo"},
      {{4, TokenType::t_dot, 11, 1, 1}},
      {{5, TokenType::t_equal, 13, 1, 1}}};
  auto project =
      std::make_shared<SourceProject>("TEST::Parser.typeNoIdentifier");
  SourceFile file{"TEST::Parser.typeNoIdentifier"};
  file.m_lines.push_back(SourceLine("let b = 5;"));
  file.m_lines.push_back(SourceLine("let a : foo. = 1;", tokens));
  file.m_lines.push_back(SourceLine("return a + b;"));
  project->addFile(std::move(file));
  auto parser = makeParser(std::move(tokens), project);

  ASSERT_EQ(parser->parseExprssion(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, parensMissingClose) {
  std::vector<TokenValuePair> tokens{{{0, TokenType::t_identifier, 0, 1}, "a"},
                                     {{1, TokenType::t_asterisk, 2, 1}},
                                     {{2, TokenType::t_open_paren, 4, 1}},
                                     {{3, TokenType::t_identifier, 5, 1}, "b"},
                                     {{4, TokenType::t_plus, 7, 1}},
                                     {{5, TokenType::t_identifier, 9, 1}, "c"},
                                     {{6, TokenType::t_semicolon, 10, 1}}};
  auto project =
      std::make_shared<SourceProject>("TEST::Parser.parensMissingClose");
  SourceFile file{"TEST::Parser.parensMissingClose"};
  file.m_lines.push_back(SourceLine("a * (b + c;", tokens));
  project->addFile(std::move(file));
  auto parser = makeParser(std::move(tokens), project);

  ASSERT_EQ(parser->parseExprssion(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, invokeMissingClose) {
  std::vector<TokenValuePair> tokens{
      {{0, TokenType::t_identifier, 0, 1, 0}, "a"},
      {{1, TokenType::t_open_paren, 1, 1, 0}},
      {{0, TokenType::t_identifier, 0, 1, 1}, "b"},
      {{1, TokenType::t_comma, 1, 1, 1}},
      {{2, TokenType::t_identifier, 3, 1, 1}, "c"},
      {{3, TokenType::t_semicolon, 4, 1, 1}}};
  auto project =
      std::make_shared<SourceProject>("TEST::Parser.invokeMissingClose");
  SourceFile file{"TEST::Parser.invokeMissingClose"};
  file.m_lines.push_back(SourceLine("a(", tokens, 0));
  file.m_lines.push_back(SourceLine("b, c;", tokens, 1));
  project->addFile(std::move(file));
  auto parser = makeParser(std::move(tokens), project);

  ASSERT_EQ(parser->parseExprssion(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, indexMissingClose) {
  std::vector<TokenValuePair> tokens{{{0, TokenType::t_identifier, 0, 1}, "a"},
                                     {{1, TokenType::t_open_square, 1, 1}},
                                     {{2, TokenType::t_semicolon, 2, 1}}};
  auto project =
      std::make_shared<SourceProject>("TEST::Parser.indexMissingClose");
  SourceFile file{"TEST::Parser.indexMissingClose"};
  file.m_lines.push_back(SourceLine("a[;", tokens));
  project->addFile(std::move(file));
  auto parser = makeParser(std::move(tokens), project);

  ASSERT_EQ(parser->parseExprssion(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, blockMissingClose) {
  std::vector<TokenValuePair> tokens{
      {{0, TokenType::t_open_bracket, 0, 1, 0}},
      {{0, TokenType::t_yield, 4, 5, 1}},
      {{1, TokenType::t_identifier, 10, 1, 1}, "a"},
      {{2, TokenType::t_semicolon, 11, 1, 1}}};
  auto project =
      std::make_shared<SourceProject>("TEST::Parser.blockMissingClose");
  SourceFile file{"TEST::Parser.blockMissingClose"};
  file.m_lines.push_back(SourceLine("{", tokens, 0));
  file.m_lines.push_back(SourceLine("    yield a;", tokens, 1));
  project->addFile(std::move(file));
  auto parser = makeParser(std::move(tokens), project);

  ASSERT_EQ(parser->parseExprssion(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}
