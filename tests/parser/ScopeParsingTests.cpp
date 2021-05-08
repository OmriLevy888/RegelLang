#include "parser/ast/constructs/NamespaceDeclarationNode.hpp"
#include "parser/ast/expressions/CompoundIdentifierNode.hpp"
#include "parser/ast/expressions/ImportNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, parseSimpleNamespaceDeclaration) {
  auto parser =
      makeParser({{TokenType::t_namespace}, {TokenType::t_identifier, "foo"s}});

  assertNode(parser->parseNamespaceDeclaration(),
             std::make_unique<NamespaceDeclarationNode>(
                 std::make_unique<BasicIdentifierNode>("foo")));
}

TEST(Parser, parseCompoundNamespaceDeclaration) {
  auto parser = makeParser({{TokenType::t_namespace},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "bar"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "baz"s}});

  assertNode(parser->parseNamespaceDeclaration(),
             std::make_unique<NamespaceDeclarationNode>(
                 std::make_unique<CompoundIdentifierNode>(
                     std::vector<std::string>{"foo", "bar", "baz"})));
}

TEST(Parser, parseSimpleImport) {
  auto parser =
      makeParser({{TokenType::t_import}, {TokenType::t_identifier, "foo"s}});

  assertNode(parser->parseExpression(),
             std::make_unique<ImportNode>(
                 std::make_unique<BasicIdentifierNode>("foo")));
}

TEST(Parser, parseCompoundImport) {
  auto parser = makeParser({{TokenType::t_import},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "bar"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "baz"s}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ImportNode>(std::make_unique<CompoundIdentifierNode>(
          std::vector<std::string>{"foo", "bar", "baz"})));
}
