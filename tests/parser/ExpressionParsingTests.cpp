#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
#include "lexer/DummyTokenGenerator.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "lexer/Token.hpp"
#include "lexer/TokenCollection.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/Type.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/StringLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/ParenthesesNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"
#include "tests/TestsCore.hpp"
#include "gtest/gtest.h"
#include <memory>

using namespace rgl;

std::unique_ptr<Parser> makeParser(std::vector<TokenValuePair> &&tokens) {
  auto tokenGenerator =
      std::make_unique<DummyTokenGenerator>(std::move(tokens));
  auto tokenCollection =
      std::make_unique<TokenCollection>(std::move(tokenGenerator));
  return std::make_unique<Parser>(std::move(tokenCollection));
}

static void assertExpr(Expression &&expr, Expression &&expected) {
  ASSERT_TRUE(expr != nullptr);
  std::cout << "===========" << std::endl;
  std::cout << expected->toString() << std::endl;
  std::cout << expr->toString() << std::endl;
  ASSERT_TRUE(expr->toString() == expected->toString());
}

TEST(Parser, identifier) {
  auto parser = makeParser(
      {{TokenType::t_identifier, "a"}, {TokenType::t_identifier, "b"}});

  assertExpr(parser->parseExprssion(), std::make_unique<IdentifierNode>("a"));
  assertExpr(parser->parseExprssion(), std::make_unique<IdentifierNode>("b"));
}

TEST(Parser, literal) {
  auto parser = makeParser({{TokenType::t_int16_literal, {15}},
                            {TokenType::t_int32_literal, {30}},
                            {TokenType::t_boolean, {true}},
                            {TokenType::t_string_literal, {"hello"}}});

  assertExpr(parser->parseExprssion(),
             std::make_unique<IntLiteralNode>(15, Type::t_int16()));
  assertExpr(parser->parseExprssion(),
             std::make_unique<IntLiteralNode>(30, Type::t_int32()));
  assertExpr(parser->parseExprssion(),
             std::make_unique<BooleanLiteralNode>(true));
  assertExpr(parser->parseExprssion(),
             std::make_unique<StringLiteralNode>("hello"));
}

TEST(Parser, precedenceNoParens) {
  auto parser = makeParser({{TokenType::t_int32_literal, {2}},
                            {TokenType::t_asterisk},
                            {TokenType::t_int32_literal, {3}},
                            {TokenType::t_plus},
                            {TokenType::t_int32_literal, {1}}});

  assertExpr(parser->parseExprssion(),
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

  assertExpr(
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

  assertExpr(
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

  assertExpr(parser->parseExprssion(),
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

  assertExpr(
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

  assertExpr(
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

  assertExpr(
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
                            {TokenType::t_ampersand},
                            {TokenType::t_identifier, "i32"}});

  assertExpr(parser->parseExprssion(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 Type::t_int32()->getReferenceType(), false, nullptr));
}

TEST(Parser, varImplicitTypeWithValue) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_equal},
                            {TokenType::t_boolean, true}});

  assertExpr(parser->parseExprssion(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"), nullptr, false,
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

  assertExpr(parser->parseExprssion(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 makeType({"foo", "bar"}), true,
                 std::make_unique<IntLiteralNode>(10, Type::t_int32())));
}
