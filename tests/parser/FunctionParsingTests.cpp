#include "lexer/Token.hpp"
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/literals/FunctionLiteral.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"
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

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLietralNode>(
                 std::make_unique<IdentifierNode>("foo"),
                 std::vector<Parameter>(), nullptr,
                 std::make_unique<BlockNode>()));
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

  assertNode(parser->parseExprssion(),
             std::make_unique<FunctionLietralNode>(
                 std::make_unique<IdentifierNode>("foo"),
                 std::vector<Parameter>(), Type::t_int32(),
                 std::make_unique<BlockNode>()));
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
             std::make_unique<FunctionLietralNode>(
                 std::make_unique<IdentifierNode>("foo"), std::move(parameters),
                 nullptr, std::make_unique<BlockNode>()));
}

TEST(Parser, functionLiteralWithNameWithParensSingleParamWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralWithNameWithParensNoParamsNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralWithNameWithParensSignleParamNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralWithNameWithParentsMultipleParamsNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralWithNameWithParensMultipleParamsSameTypeNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralNoNameNoParensSingleParamNoRetType) {
  auto parser = makeParser({{TokenType::t_func},

                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralNoNameWithParensSingleParamNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralNoNameWithParensSingleParamNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralNoNameNoParensSingleParamWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralNoNameNoParensNoParamsWithRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralNoNameNoParensNoParamsNoRetTypeNoBrackets) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
TEST(Parser, functionLiteralNoNameNoParensNoParamsNoRetType) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});
}
