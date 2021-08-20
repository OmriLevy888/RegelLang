#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/constructs/BasicTypeNode.hpp"
#include "parser/ast/constructs/FunctionTypeNode.hpp"
#include "parser/ast/expressions/ScopeNode.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/statements/ReturnNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"
#include "gtest/gtest.h"

using namespace rgl;

TEST(Parser, primitiveTypeParsing) {
  auto parser = makeParser({{TokenType::t_identifier, "void"s},
                            {TokenType::t_identifier, "i8"s},
                            {TokenType::t_identifier, "i16"s},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_identifier, "i64"s},
                            {TokenType::t_identifier, "u8"s},
                            {TokenType::t_identifier, "u16"s},
                            {TokenType::t_identifier, "u32"s},
                            {TokenType::t_identifier, "u64"s},
                            {TokenType::t_identifier, "float"s},
                            {TokenType::t_identifier, "double"s},
                            {TokenType::t_identifier, "char"s},
                            {TokenType::t_identifier, "string"s},
                            {TokenType::t_identifier, "bool"s}});

  std::vector<TypeNodePtr> types;
  types.reserve(14);
  types.push_back(BasicTypeNode::t_void());
  types.push_back(BasicTypeNode::t_int8());
  types.push_back(BasicTypeNode::t_int16());
  types.push_back(BasicTypeNode::t_int32());
  types.push_back(BasicTypeNode::t_int64());
  types.push_back(BasicTypeNode::t_uint8());
  types.push_back(BasicTypeNode::t_uint16());
  types.push_back(BasicTypeNode::t_uint32());
  types.push_back(BasicTypeNode::t_uint64());
  types.push_back(BasicTypeNode::t_float());
  types.push_back(BasicTypeNode::t_double());
  types.push_back(BasicTypeNode::t_char());
  types.push_back(BasicTypeNode::t_string());
  types.push_back(BasicTypeNode::t_bool());
  for (const auto &type : types) {
    ASSERT_EQ(parser->parseType()->toString(), type->toString());
  }
}

TEST(Parser, compoundTypeParsing) {
  auto parser = makeParser({{TokenType::t_identifier, "a"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "c"s}});

  ASSERT_EQ(
      parser->parseType()->toString(),
      std::make_unique<BasicTypeNode>(std::vector<std::string>{"a", "b", "c"})
          ->toString());
}

TEST(Parser, functionTypeParsing) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"s},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "float"s},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "bool"s}});

  std::vector<TypeNodePtr> paramTypes;
  paramTypes.reserve(2);
  paramTypes.push_back(BasicTypeNode::t_int32());
  paramTypes.push_back(BasicTypeNode::t_float());
  ASSERT_EQ(parser->parseType()->toString(),
            std::make_unique<FunctionTypeNode>(std::move(paramTypes),
                                               BasicTypeNode::t_bool())
                ->toString());
}

TEST(Parser, pointerTypeParsing) {
  auto parser = makeParser({{TokenType::t_lesser_than},
                            {TokenType::t_greater_than},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket},
                            {TokenType::t_identifier, "b"s}});

  std::vector<TypeNodePtr> types;
  types.reserve(2);
  types.push_back(std::make_unique<BasicTypeNode>(std::vector<std::string>{"a"},
                                                  TypeProperties::_isPointer));
  types.push_back(std::make_unique<BasicTypeNode>(
      std::vector<std::string>{"b"},
      BitField<TypeProperties>{TypeProperties::_isPointer} |
          TypeProperties::_isShared));
  for (const auto &type : types) {
    ASSERT_EQ(parser->parseType()->toString(), type->toString());
  }
}

TEST(Parser, containerTypeParsing) {
  // TODO: implement when container types are a things
}

TEST(Parser, typeModifiersParsing) {
  auto parser = makeParser({{TokenType::t_identifier, "a"s},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_ampersand},
                            {TokenType::t_identifier, "a"s}});

  std::vector<TypeNodePtr> types;
  types.reserve(3);
  types.push_back(
      std::make_unique<BasicTypeNode>(std::vector<std::string>{"a"}));
  types.push_back(std::make_unique<BasicTypeNode>(std::vector<std::string>{"a"},
                                                  TypeProperties::_owning));
  types.push_back(std::make_unique<BasicTypeNode>(std::vector<std::string>{"a"},
                                                  TypeProperties::_mutable));
  for (const auto &type : types) {
    ASSERT_EQ(parser->parseType()->toString(), type->toString());
  }
}

TEST(Parser, functionTypeMissingComma) {
  std::vector<TokenValuePair> tokens;

  auto parser = makeParser("TEST::Parser.functionTypeMissingComma",
                           {{{TokenType::t_func, {0, 0, 0, 4}}},
                            {{TokenType::t_open_paren, {0, 0, 5, 1}}},
                            {{TokenType::t_identifier, {0, 0, 6, 3}}, "i32"s},
                            {{TokenType::t_identifier, {0, 0, 10, 5}}, "float"s},
                            {{TokenType::t_close_paren, {0, 0, 16, 1}}}},
                           {"func (i32 float)"});

  ASSERT_EQ(parser->parseType(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Paresr, typeInequality) {
  ASSERT_NE(BasicTypeNode::t_int32(), BasicTypeNode::t_double());
  ASSERT_NE(std::make_unique<BasicTypeNode>(std::vector<std::string>{"a"}),
            std::make_unique<BasicTypeNode>(std::vector<std::string>{"b"}));
  ASSERT_NE(BasicTypeNode::t_bool(),
            BasicTypeNode::t_bool()->getSharedPointerType());
}
