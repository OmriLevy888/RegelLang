#include <memory>

#include "common/collections/source-objects/SourceLine.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "common/errors/ErrorManager.hpp"
#include "gtest/gtest.h"
#include "lexer/DummyTokenGenerator.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/expressions/literals/CharLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/StringLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/statements/BreakNode.hpp"
#include "parser/ast/statements/ContinueNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/ReturnNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, keywordStatement) {
  auto parser = makeParser({{TokenType::t_continue},
                            {TokenType::t_semicolon},
                            {TokenType::t_break},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseStatement(), std::make_unique<ContinueNode>());
  assertNode(parser->parseStatement(), std::make_unique<BreakNode>());
}

TEST(Parser, simpleStatement) {
  auto parser = makeParser({{TokenType::t_yield},
                            {TokenType::t_int32_literal, 5},
                            {TokenType::t_plus},
                            {TokenType::t_int32_literal, 5},
                            {TokenType::t_semicolon},
                            {TokenType::t_return},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseStatement(),
             std::make_unique<YieldNode>(std::make_unique<BinOpNode>(
                 BinOpType::b_plus,
                 std::make_unique<IntLiteralNode>(5, BasicType::t_int32()),
                 std::make_unique<IntLiteralNode>(5, BasicType::t_int32()))));
  assertNode(parser->parseStatement(), std::make_unique<ReturnNode>());
}

TEST(Parser, expressionStatement) {
  auto parser = makeParser({{TokenType::t_string_literal, "hello world"},
                            {TokenType::t_plus},
                            {TokenType::t_char_literal, '!'},
                            {TokenType::t_semicolon}});

  assertNode(
      parser->parseStatement(),
      std::make_unique<ExpressionStatementNode>(std::make_unique<BinOpNode>(
          BinOpType::b_plus, std::make_unique<StringLiteralNode>("hello world"),
          std::make_unique<CharLiteralNode>('!'))));
}

TEST(Parser, expressionStatementMissingSemicolonError) {
  std::vector<TokenValuePair> tokens{
      {{0, TokenType::t_identifier, 0, 1, 0}, "a"},
      {{0, TokenType::t_for, 0, 3, 1}}};
  auto project = std::make_shared<SourceProject>(
      "TEST::Parser.expressionStatementMissingSemicolonError");
  SourceFile file{"TEST::Parser.expressionStatementMissingSemicolonError"};
  file.m_lines.push_back(SourceLine("a", tokens, 0));
  file.m_lines.push_back(SourceLine("for", tokens, 1));
  project->addFile(std::move(file));
  auto parser = makeParser(std::move(tokens), project);

  ASSERT_EQ(parser->parseStatement(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, keywordStatementMissingSemicolonError) {
  std::vector<TokenValuePair> tokens{{{0, TokenType::t_break, 4, 5, 1}},
                                     {{1, TokenType::t_eof, 9, 1, 1}}};
  auto project = std::make_shared<SourceProject>(
      "TEST::Parser.keywordStatementMissingSemicolonError");
  SourceFile file{"TEST::Parser.keywordStatementMissingSemicolonError"};
  file.m_lines.push_back(
      SourceLine("for var idx = 0; idx < 3; idx++", tokens, 0));
  file.m_lines.push_back(SourceLine("    break", tokens, 1));
  project->addFile(std::move(file));
  auto parser = makeParser(std::move(tokens), project);

  ASSERT_EQ(parser->parseStatement(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, simpleStatementMissingSemicolonError) {
  std::vector<TokenValuePair> tokens{
      {{0, TokenType::t_return, 4, 6, 0}},
      {{1, TokenType::t_identifier, 11, 1, 0}, "a"},
      {{0, TokenType::t_close_bracket, 0, 1, 1}}};
  auto project = std::make_shared<SourceProject>(
      "TEST::Parser.simpleStatementMissingSemicolonError");
  SourceFile file{"TEST::Parser.simpleStatementMissingSemicolonError"};
  file.m_lines.push_back(SourceLine("    return a", tokens, 0));
  file.m_lines.push_back(SourceLine("}", tokens, 1));
  project->addFile(std::move(file));
  auto parser = makeParser(std::move(tokens), project);

  ASSERT_EQ(parser->parseStatement(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}
