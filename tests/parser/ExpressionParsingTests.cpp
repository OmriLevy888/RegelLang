#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
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
                            {TokenType::t_ampersand},
                            {TokenType::t_identifier, "i32"}});

  assertNode(parser->parseExprssion(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 Type::t_int32()->getReferenceType(), false, nullptr));
}

TEST(Parser, varImplicitTypeWithValue) {
  auto parser = makeParser({{TokenType::t_var},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_equal},
                            {TokenType::t_boolean, true}});

  assertNode(parser->parseExprssion(),
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

  assertNode(parser->parseExprssion(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"),
                 makeType({"foo", "bar"}), true,
                 std::make_unique<IntLiteralNode>(10, Type::t_int32())));
}

TEST(Parser, emptyBlock) {
  auto parser = makeParser({{TokenType::t_let},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_equal},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExprssion(),
             std::make_unique<VarDeclNode>(
                 std::make_unique<IdentifierNode>("a"), nullptr, true,
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
                 std::make_unique<IdentifierNode>("a"), makeType({"b"}), false,
                 std::make_unique<BinOpNode>(
                     BinOpType::b_plus,
                     std::make_unique<IntLiteralNode>(1, Type::t_int32()),
                     std::make_unique<BlockNode>(std::move(statements)))));
}

TEST(Parser, simpleIf) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_open_bracket},
                            {TokenType::t_boolean, true},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExprssion(),
             std::make_unique<ConditionalNode>(
                 std::make_unique<BooleanLiteralNode>(false),
                 std::make_unique<ExpressionStatementNode>(
                     std::make_unique<BooleanLiteralNode>(true))));
}

TEST(Parser, sinlgeStatementIf) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_int32_literal, 5},
                            {TokenType::t_int8_literal, 1},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseExprssion(),
             std::make_unique<ConditionalNode>(
                 std::make_unique<IntLiteralNode>(5, Type::t_int32()),
                 std::make_unique<ExpressionStatementNode>(
                     std::make_unique<IntLiteralNode>(1, Type::t_int8()))));
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
                            {TokenType::t_int32_literal, 0},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<ConditionalNode>(
          std::make_unique<BooleanLiteralNode>(true),
          std::make_unique<ExpressionStatementNode>(
              std::make_unique<BooleanLiteralNode>(false)),
          std::make_unique<ConditionalNode>(
              std::make_unique<BooleanLiteralNode>(true),
              std::make_unique<YieldNode>(
                  std::make_unique<IntLiteralNode>(0, Type::t_int32())))));
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
                            {TokenType::t_int32_literal, 0},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseExprssion(),
             std::make_unique<ConditionalNode>(
                 std::make_unique<BooleanLiteralNode>(true),
                 std::make_unique<YieldNode>(
                     std::make_unique<BooleanLiteralNode>(true)),
                 std::make_unique<ExpressionStatementNode>(
                     std::make_unique<IntLiteralNode>(0, Type::t_int32()))));
}

TEST(Parser, multipleElifsAndElse) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_int32_literal, 0},
                            {TokenType::t_semicolon},
                            {TokenType::t_elif},
                            {TokenType::t_boolean, false},
                            {TokenType::t_int32_literal, 1},
                            {TokenType::t_semicolon},
                            {TokenType::t_elif},
                            {TokenType::t_boolean, true},
                            {TokenType::t_int32_literal, 2},
                            {TokenType::t_semicolon},
                            {TokenType::t_else},
                            {TokenType::t_int32_literal, 3},
                            {TokenType::t_semicolon}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<ConditionalNode>(
          std::make_unique<BooleanLiteralNode>(false),
          std::make_unique<ExpressionStatementNode>(
              std::make_unique<IntLiteralNode>(0, Type::t_int32())),
          std::make_unique<ConditionalNode>(
              std::make_unique<BooleanLiteralNode>(false),
              std::make_unique<ExpressionStatementNode>(
                  std::make_unique<IntLiteralNode>(1, Type::t_int32())),
              std::make_unique<ConditionalNode>(
                  std::make_unique<BooleanLiteralNode>(true),
                  std::make_unique<ExpressionStatementNode>(
                      std::make_unique<IntLiteralNode>(2, Type::t_int32())),
                  std::make_unique<ExpressionStatementNode>(
                      std::make_unique<IntLiteralNode>(3, Type::t_int32()))))));
}

TEST(Parser, compoundElif) {
  auto parser = makeParser({{TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_int32_literal, 0},
                            {TokenType::t_semicolon},
                            {TokenType::t_elif},
                            {TokenType::t_boolean, true},
                            {TokenType::t_if},
                            {TokenType::t_boolean, false},
                            {TokenType::t_int32_literal, 1},
                            {TokenType::t_semicolon},
                            {TokenType::t_elif},
                            {TokenType::t_boolean, true},
                            {TokenType::t_int32_literal, 2},
                            {TokenType::t_semicolon}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<ConditionalNode>(
          std::make_unique<BooleanLiteralNode>(false),
          std::make_unique<ExpressionStatementNode>(
              std::make_unique<IntLiteralNode>(0, Type::t_int32())),
          std::make_unique<ConditionalNode>(
              std::make_unique<BooleanLiteralNode>(true),
              std::make_unique<ExpressionStatementNode>(
                  std::make_unique<ConditionalNode>(
                      std::make_unique<BooleanLiteralNode>(false),
                      std::make_unique<ExpressionStatementNode>(
                          std::make_unique<IntLiteralNode>(1, Type::t_int32())),
                      std::make_unique<ConditionalNode>(
                          std::make_unique<BooleanLiteralNode>(true),
                          std::make_unique<ExpressionStatementNode>(
                              std::make_unique<IntLiteralNode>(
                                  2, Type::t_int32()))))))));
}
