#include "lexer/Token.hpp"
#include "parser/ast/constructs/BasicType.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "parser/ast/expressions/literals/class-literal/ClassLiteralNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, classEmptyDefinition) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"),
                 std::vector<FieldPtr>{}, std::vector<MethodPtr>{}));
}

TEST(Parser, classSingleMethod) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_bracket},

                            {TokenType::t_func},
                            {TokenType::t_identifier, "bar"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket},

                            {TokenType::t_close_bracket}});

  std::vector<MethodPtr> methods{};
  methods.push_back(std::make_unique<MethodNode>(
      false,
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("bar"), std::vector<Parameter>{},
          BasicType::t_implicit(), std::make_unique<BlockNode>()),
      MethodProperties::_default));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"),
                 std::vector<FieldPtr>{}, std::move(methods)));
}

TEST(Parser, classMultipleMethods) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_bracket},

                            {TokenType::t_func},
                            {TokenType::t_identifier, "bar"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket},

                            {TokenType::t_func},
                            {TokenType::t_identifier, "baz"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket},

                            {TokenType::t_close_bracket}});

  std::vector<MethodPtr> methods{};
  methods.push_back(std::make_unique<MethodNode>(
      false,
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("bar"), std::vector<Parameter>{},
          BasicType::t_implicit(), std::make_unique<BlockNode>()),
      MethodProperties::_default));
  methods.push_back(std::make_unique<MethodNode>(
      false,
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("baz"), std::vector<Parameter>{},
          BasicType::t_implicit(), std::make_unique<BlockNode>()),
      MethodProperties::_default));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"),
                 std::vector<FieldPtr>{}, std::move(methods)));
}

TEST(Parser, classSingleField) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_bracket},

                            {TokenType::t_identifier, "a"},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "bool"},
                            {TokenType::t_semicolon},

                            {TokenType::t_close_bracket}});

  std::vector<FieldPtr> fields{};
  fields.push_back(std::make_unique<ClassFieldNode>(
      false, BasicType::t_bool()->getOwningType(),
      std::make_unique<IdentifierNode>("a")));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"), std::move(fields),
                 std::vector<MethodPtr>{}));
}

TEST(Parser, classMultipleFields) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_bracket},

                            {TokenType::t_var},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "bool"},
                            {TokenType::t_comma},

                            {TokenType::t_identifier, "b"},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "double"},
                            {TokenType::t_semicolon},

                            {TokenType::t_close_bracket}});

  std::vector<FieldPtr> fields{};
  fields.push_back(std::make_unique<ClassFieldNode>(
      false, BasicType::t_bool()->getOwningType()->getMutableType(),
      std::make_unique<IdentifierNode>("a")));
  fields.push_back(std::make_unique<ClassFieldNode>(
      false, BasicType::t_double()->getOwningType()->getMutableType(),
      std::make_unique<IdentifierNode>("b")));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"), std::move(fields),
                 std::vector<MethodPtr>{}));
}

TEST(Parser, classPublicMembers) {}

TEST(Parser, classMultipleFieldsDeclaration) {}

TEST(Parser, classMultipleFieldsShorthand) {}
