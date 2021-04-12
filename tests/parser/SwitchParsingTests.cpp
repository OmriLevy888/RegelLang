#include <memory>

#include "lexer/Token.hpp"
#include "parser/ast/constructs/Type.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/SwitchCaseNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, simpleSwitchCase) {
  auto parser = makeParser({{TokenType::t_switch},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_int32_literal, 0},
                            {TokenType::t_arrow},
                            {TokenType::t_yield},
                            {TokenType::t_boolean, true},
                            {TokenType::t_semicolon},
                            {TokenType::t_int32_literal, 1},
                            {TokenType::t_arrow},
                            {TokenType::t_yield},
                            {TokenType::t_boolean, false},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  std::vector<SwitchCase> cases;
  cases.push_back(std::make_unique<SwitchCaseNode>(
      std::make_unique<IntLiteralNode>(0, BasicType::t_int32()),
      std::make_unique<YieldNode>(std::make_unique<BooleanLiteralNode>(true))));

  cases.push_back(std::make_unique<SwitchCaseNode>(
      std::make_unique<IntLiteralNode>(1, BasicType::t_int32()),
      std::make_unique<YieldNode>(
          std::make_unique<BooleanLiteralNode>(false))));

  assertNode(parser->parseExpression(),
             std::make_unique<SwitchNode>(std::make_unique<IdentifierNode>("a"),
                                          std::move(cases)));
}

TEST(Parser, switchCaseWithDefaultStatement) {
  auto parser = makeParser({{TokenType::t_switch},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_int32_literal, 0},
                            {TokenType::t_arrow},
                            {TokenType::t_yield},
                            {TokenType::t_boolean, true},
                            {TokenType::t_semicolon},
                            {TokenType::t_int32_literal, 1},
                            {TokenType::t_arrow},
                            {TokenType::t_yield},
                            {TokenType::t_boolean, true},
                            {TokenType::t_semicolon},
                            {TokenType::t_identifier, "_"},
                            {TokenType::t_arrow},
                            {TokenType::t_yield},
                            {TokenType::t_boolean, false},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  std::vector<SwitchCase> cases;
  cases.push_back(std::make_unique<SwitchCaseNode>(
      std::make_unique<IntLiteralNode>(0, BasicType::t_int32()),
      std::make_unique<YieldNode>(std::make_unique<BooleanLiteralNode>(true))));

  cases.push_back(std::make_unique<SwitchCaseNode>(
      std::make_unique<IntLiteralNode>(1, BasicType::t_int32()),
      std::make_unique<YieldNode>(std::make_unique<BooleanLiteralNode>(true))));

  cases.push_back(std::make_unique<SwitchCaseNode>(
      std::make_unique<IdentifierNode>("_"),
      std::make_unique<YieldNode>(
          std::make_unique<BooleanLiteralNode>(false))));

  assertNode(parser->parseExpression(),
             std::make_unique<SwitchNode>(std::make_unique<IdentifierNode>("a"),
                                          std::move(cases)));
}
