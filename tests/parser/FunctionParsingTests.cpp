#include "lexer/Token.hpp"
#include "parser/ast/constructs/BasicType.hpp"
#include "parser/ast/expressions/BasicIdentifierNode.hpp"
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/statements/ReturnNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, functionLiteralWithNameWithParensNoParamsNoRetTypeEmptyBody) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<BasicIdentifierNode>("foo"),
                 std::vector<Parameter>(), BasicType::t_void(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensNoParamsWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<BasicIdentifierNode>("foo"),
                 std::vector<Parameter>(), BasicType::t_int32(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensSingleParamNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("a")));

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<BasicIdentifierNode>("foo"),
                 std::move(parameters), BasicType::t_void(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensSingleParamWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("a")));

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<BasicIdentifierNode>("foo"),
                 std::move(parameters), BasicType::t_int32(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensNoParamsNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<BasicIdentifierNode>("foo"),
                 std::vector<Parameter>(), BasicType::t_int32(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensSingleParamNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_close_paren},
                            {TokenType::t_return},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_asterisk},
                            {TokenType::t_int32_literal, 2l},
                            {TokenType::t_semicolon}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("a")));

  std::vector<Statement> statements;
  statements.push_back(std::make_unique<ReturnNode>(std::make_unique<BinOpNode>(
      BinOpType::b_asterisk, std::make_unique<BasicIdentifierNode>("a"),
      std::make_unique<IntLiteralNode>(2, BasicType::t_int32()))));

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<BasicIdentifierNode>("foo"),
                 std::move(parameters), BasicType::t_void(),
                 std::make_unique<ScopeNode>(std::move(statements))));
}

TEST(Parser, functionLiteralWithNameWithParentsMultipleParamsNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("a")));
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("b")));

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<BasicIdentifierNode>("foo"),
                 std::move(parameters), BasicType::t_void(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensMultipleParamsSameTypeNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("a")));
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("b")));

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<BasicIdentifierNode>("foo"),
                 std::move(parameters), BasicType::t_void(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralNoNameNoParensSingleParamNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("a")));

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::move(parameters), BasicType::t_void(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralNoNameWithParensSingleParamNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_close_paren},
                            {TokenType::t_return},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_asterisk},
                            {TokenType::t_int32_literal, 2l},
                            {TokenType::t_semicolon}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("a")));
  std::vector<Statement> statements;
  statements.push_back(std::make_unique<ReturnNode>(std::make_unique<BinOpNode>(
      BinOpType::b_asterisk, std::make_unique<BasicIdentifierNode>("a"),
      std::make_unique<IntLiteralNode>(2, BasicType::t_int32()))));

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::move(parameters), BasicType::t_void(),
                 std::make_unique<ScopeNode>(std::move(statements))));
}

TEST(Parser, functionLiteralNoNameWithParensSingleParamNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("a")));

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::move(parameters), BasicType::t_void(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralNoNameNoParensSingleParamWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int32(), std::make_unique<BasicIdentifierNode>("a")));

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::move(parameters), BasicType::t_int32(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralNoNameNoParensNoParams) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::vector<Parameter>{}, BasicType::t_int32(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralNoNameNoParensNoParamsNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::vector<Parameter>{}, BasicType::t_void(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralNoParensNoParamsWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::vector<Parameter>{}, BasicType::t_int32(),
                 std::make_unique<ScopeNode>()));
}

TEST(Parser, functionLiteralMissingParameterName) {
  auto parser = makeParser("TEST::Parser.functionLiteralMissingParameterName",
                           {{{0, TokenType::t_func, 0, 4}},
                            {{1, TokenType::t_identifier, 5, 3}, "foo"s},
                            {{2, TokenType::t_open_paren, 8, 1}},
                            {{3, TokenType::t_identifier, 9, 3}, "i32"s},
                            {{4, TokenType::t_close_paren, 12, 1}},
                            {{5, TokenType::t_open_bracket, 14, 1}},
                            {{6, TokenType::t_close_bracket, 15, 1}}},
                           {"func foo(i32) {}"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, functionLiteralSingleParamNoParensNoBrackets) {
  auto parser =
      makeParser("TEST::Parser.functionLiteralSingleParamNoParensNoBrackets",
                 {{{0, TokenType::t_func, 0, 4}},
                  {{1, TokenType::t_identifier, 5, 3}, "i32"s},
                  {{2, TokenType::t_identifier, 9, 1}, "a"s},
                  {{3, TokenType::t_return, 11, 6}},
                  {{4, TokenType::t_identifier, 18, 1}, "a"s},
                  {{5, TokenType::t_semicolon, 19, 1}}},
                 {"func i32 a return a;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, functionLiteralRetTypeNoBrackets) {
  auto parser = makeParser("TEST::Parser.functionLiteralRetTypeNoBrackets",
                           {{{0, TokenType::t_func, 0, 4}},
                            {{1, TokenType::t_open_paren, 5, 1}},
                            {{2, TokenType::t_close_paren, 6, 1}},
                            {{3, TokenType::t_arrow, 8, 2}},
                            {{4, TokenType::t_identifier, 11, 3}, "i32"s},
                            {{5, TokenType::t_return, 15, 6}},
                            {{6, TokenType::t_identifier, 21, 1}, "a"s},
                            {{7, TokenType::t_semicolon, 22, 1}}},
                           {"func () => i32 return 5;"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, functionLiteralMissingParameterComma) {
  auto parser = makeParser("TEST::Parser.functionLiteralMissingParameterComma",
                           {{{0, TokenType::t_func, 0, 4}},
                            {{1, TokenType::t_open_paren, 5, 1}},
                            {{2, TokenType::t_identifier, 6, 3}, "u64"s},
                            {{3, TokenType::t_identifier, 10, 1}, "a"s},
                            {{4, TokenType::t_identifier, 12, 3}, "i64"s},
                            {{5, TokenType::t_identifier, 16, 1}, "b"s},
                            {{6, TokenType::t_close_paren, 17, 1}},
                            {{7, TokenType::t_open_bracket, 19, 1}},
                            {{7, TokenType::t_close_bracket, 20, 1}}},
                           {"func (u64 a i64 b) {}"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, functionLiteralMissingCloseParen) {
  auto parser = makeParser("TEST::Parser.functionLiteralMissingCloseParen",
                           {{{TokenType::t_func, 0, 4}},
                            {{TokenType::t_open_paren, 5, 1}},
                            {{TokenType::t_identifier, 6, 5}, "float"s},
                            {{TokenType::t_identifier, 12, 1}, "a"s},
                            {{TokenType::t_open_bracket, 14, 1}},
                            {{TokenType::t_close_bracket, 15, 1}}},
                           {"func (float a {}"});

  ASSERT_EQ(parser->parseExpression(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}
