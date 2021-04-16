#include "parser/ast/expressions/CompoundIdentifierNode.hpp"
#include "parser/ast/expressions/scope/ImportNode.hpp"
#include "parser/ast/expressions/scope/NamespaceDeclarationNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, parseSimpleNamespaceDeclaration) {
  auto parser =
      makeParser({{TokenType::t_namespace}, {TokenType::t_identifier, "foo"}});

  assertNode(parser->parseExpression(),
             std::make_unique<NamespaceDeclarationNode>(
                 std::make_unique<BasicIdentifierNode>("foo")));
}

TEST(Parser, parseCompoundNamespaceDeclaration) {
  auto parser = makeParser({{TokenType::t_namespace},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "bar"},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "baz"}});

  assertNode(parser->parseExpression(),
             std::make_unique<NamespaceDeclarationNode>(
                 std::make_unique<CompoundIdentifierNode>(
                     std::vector<std::string>{"foo", "bar", "baz"})));
}

TEST(Parser, parseSimpleImport) {
  auto parser =
      makeParser({{TokenType::t_import}, {TokenType::t_identifier, "foo"}});

  assertNode(parser->parseExpression(),
             std::make_unique<ImportNode>(
                 std::make_unique<BasicIdentifierNode>("foo")));
}

TEST(Parser, parseCompoundImport) {
  auto parser = makeParser({{TokenType::t_import},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "bar"},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "baz"}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ImportNode>(std::make_unique<CompoundIdentifierNode>(
          std::vector<std::string>{"foo", "bar", "baz"})));
}
