#include "lexer/Token.hpp"
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
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("foo"), std::vector<Parameter>(),
          nullptr, std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensNoParamsWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("foo"), std::vector<Parameter>(),
          Type::t_int32(), std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensSingleParamNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("a")));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"), std::move(parameters),
                 nullptr, std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensSingleParamWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("a")));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"), std::move(parameters),
                 Type::t_int32(), std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensNoParamsNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExprssion(),
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("foo"), std::vector<Parameter>(),
          Type::t_int32(), std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensSignleParamNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_close_paren},
                            {TokenType::t_return},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_asterisk},
                            {TokenType::t_int32_literal, 2},
                            {TokenType::t_semicolon}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("a")));

  std::vector<Statement> statements;
  statements.push_back(std::make_unique<ReturnNode>(std::make_unique<BinOpNode>(
      BinOpType::b_asterisk, std::make_unique<IdentifierNode>("a"),
      std::make_unique<IntLiteralNode>(2, Type::t_int32()))));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"), std::move(parameters),
                 nullptr, std::make_unique<BlockNode>(std::move(statements))));
}

TEST(Parser, functionLiteralWithNameWithParentsMultipleParamsNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("a")));
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("b")));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"), std::move(parameters),
                 nullptr, std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensMultipleParamsSameTypeNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("a")));
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("b")));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"), std::move(parameters),
                 nullptr, std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralNoNameNoParensSingleParamNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("a")));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::move(parameters), nullptr,
                 std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralNoNameWithParensSingleParamNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_close_paren},
                            {TokenType::t_return},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_asterisk},
                            {TokenType::t_int32_literal, 2},
                            {TokenType::t_semicolon}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("a")));
  std::vector<Statement> statements;
  statements.push_back(std::make_unique<ReturnNode>(std::make_unique<BinOpNode>(
      BinOpType::b_asterisk, std::make_unique<IdentifierNode>("a"),
      std::make_unique<IntLiteralNode>(2, Type::t_int32()))));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::move(parameters), nullptr,
                 std::make_unique<BlockNode>(std::move(statements))));
}

TEST(Parser, functionLiteralNoNameWithParensSingleParamNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("a")));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::move(parameters), nullptr,
                 std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralNoNameNoParensSingleParamWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  std::vector<Parameter> parameters;
  parameters.push_back(std::make_unique<ParameterNode>(
      Type::t_int32(), std::make_unique<IdentifierNode>("a")));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::move(parameters), Type::t_int32(),
                 std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralNoNameNoParensNoParams) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::vector<Parameter>{}, Type::t_int32(),
                 std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralNoNameNoParensNoParamsNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_return},
                            {TokenType::t_int32_literal, 5},
                            {TokenType::t_semicolon}});

  std::vector<Statement> statements;
  statements.push_back(std::make_unique<ReturnNode>(
      std::make_unique<IntLiteralNode>(5, Type::t_int32())));

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::vector<Parameter>{}, nullptr,
                 std::make_unique<BlockNode>(std::move(statements))));
}

TEST(Parser, functionLiteralNoNameNoParensNoParamsNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::vector<Parameter>{}, nullptr,
                 std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralNoParensNoParamsWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLiteralNode>(
                 nullptr, std::vector<Parameter>{}, Type::t_int32(),
                 std::make_unique<BlockNode>()));
}
