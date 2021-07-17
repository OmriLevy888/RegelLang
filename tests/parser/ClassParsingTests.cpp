#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/constructs/BasicTypeNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "parser/ast/expressions/literals/class-literal/ClassLiteralNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, classEmptyDefinition) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket}});

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"s),
                 std::vector<FieldPtr>{}, std::vector<MethodPtr>{}));
}

TEST(Parser, classSingleMethod) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_bracket},

                            {TokenType::t_func},
                            {TokenType::t_identifier, "bar"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket},

                            {TokenType::t_close_bracket}});

  std::vector<MethodPtr> methods{};
  methods.push_back(std::make_unique<MethodNode>(
      false,
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("bar"s), std::vector<Parameter>{},
          BasicTypeNode::t_void(), std::make_unique<ScopeNode>()),
      MethodProperties::_default));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"s),
                 std::vector<FieldPtr>{}, std::move(methods)));
}

TEST(Parser, classMultipleMethods) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_bracket},

                            {TokenType::t_func},
                            {TokenType::t_identifier, "bar"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket},

                            {TokenType::t_func},
                            {TokenType::t_identifier, "baz"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket},

                            {TokenType::t_close_bracket}});

  std::vector<MethodPtr> methods{};
  methods.push_back(std::make_unique<MethodNode>(
      false,
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("bar"s), std::vector<Parameter>{},
          BasicTypeNode::t_void(), std::make_unique<ScopeNode>()),
      MethodProperties::_default));
  methods.push_back(std::make_unique<MethodNode>(
      false,
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("baz"s), std::vector<Parameter>{},
          BasicTypeNode::t_void(), std::make_unique<ScopeNode>()),
      MethodProperties::_default));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"s),
                 std::vector<FieldPtr>{}, std::move(methods)));
}

TEST(Parser, classSingleField) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_bracket},

                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "bool"s},
                            {TokenType::t_semicolon},

                            {TokenType::t_close_bracket}});

  std::vector<FieldPtr> fields{};
  fields.push_back(std::make_unique<ClassFieldNode>(
      false, BasicTypeNode::t_bool()->getOwningType(),
      std::make_unique<IdentifierNode>("a"s)));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"s), std::move(fields),
                 std::vector<MethodPtr>{}));
}

TEST(Parser, classMultipleFields) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_bracket},

                            {TokenType::t_var},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "bool"s},
                            {TokenType::t_comma},

                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "double"s},
                            {TokenType::t_semicolon},

                            {TokenType::t_close_bracket}});

  std::vector<FieldPtr> fields{};
  fields.push_back(std::make_unique<ClassFieldNode>(
      false, BasicTypeNode::t_bool()->getOwningType()->getMutableType(),
      std::make_unique<IdentifierNode>("a"s)));
  fields.push_back(std::make_unique<ClassFieldNode>(
      false, BasicTypeNode::t_double()->getOwningType()->getMutableType(),
      std::make_unique<IdentifierNode>("b"s)));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"s), std::move(fields),
                 std::vector<MethodPtr>{}));
}

TEST(Parser, classPublicMembers) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_bracket},

                            {TokenType::t_pub},
                            {TokenType::t_identifier, "bar"s},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "u8"s},
                            {TokenType::t_semicolon},

                            {TokenType::t_func},
                            {TokenType::t_identifier, "baz"s},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket},

                            {TokenType::t_close_bracket}});

  std::vector<FieldPtr> fields{};
  fields.push_back(std::make_unique<ClassFieldNode>(
      true, BasicTypeNode::t_uint8()->getOwningType()->getMutableType(),
      std::make_unique<IdentifierNode>("bar"s)));

  std::vector<MethodPtr> methods{};
  methods.push_back(std::make_unique<MethodNode>(
      false,
      std::make_unique<FunctionLiteralNode>(
          std::make_unique<IdentifierNode>("baz"s), std::vector<Parameter>{},
          BasicTypeNode::t_void(), std::make_unique<ScopeNode>()),
      MethodProperties::_default));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"s), std::move(fields),
                 std::move(methods)));
}

TEST(Parser, classMultipleFieldsShorthand) {
  auto parser = makeParser({{TokenType::t_class},
                            {TokenType::t_identifier, "foo"s},
                            {TokenType::t_open_bracket},

                            {TokenType::t_var},
                            {TokenType::t_open_square},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "c"s},
                            {TokenType::t_close_square},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_semicolon},

                            {TokenType::t_close_bracket}});

  std::vector<FieldPtr> fields{};
  fields.push_back(std::make_unique<ClassFieldNode>(
      false, BasicTypeNode::t_int32()->getOwningType()->getMutableType(),
      std::make_unique<IdentifierNode>("a"s)));
  fields.push_back(std::make_unique<ClassFieldNode>(
      false, BasicTypeNode::t_int32()->getOwningType()->getMutableType(),
      std::make_unique<IdentifierNode>("b"s)));
  fields.push_back(std::make_unique<ClassFieldNode>(
      false, BasicTypeNode::t_int32()->getOwningType()->getMutableType(),
      std::make_unique<IdentifierNode>("c"s)));

  assertNode(parser->parseExpression(),
             std::make_unique<ClassLiteralNode>(
                 std::make_unique<IdentifierNode>("foo"s), std::move(fields),
                 std::vector<MethodPtr>{}));
}
