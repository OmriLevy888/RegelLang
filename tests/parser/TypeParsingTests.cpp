#include "lexer/Token.hpp"
#include "parser/ast/constructs/FunctionType.hpp"
#include "parser/ast/constructs/Type.hpp"
#include "parser/ast/expressions/BlockNode.hpp"
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
  auto parser = makeParser({{TokenType::t_identifier, "void"},
                            {TokenType::t_identifier, "i8"},
                            {TokenType::t_identifier, "i16"},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "i64"},
                            {TokenType::t_identifier, "u8"},
                            {TokenType::t_identifier, "u16"},
                            {TokenType::t_identifier, "u32"},
                            {TokenType::t_identifier, "u64"},
                            {TokenType::t_identifier, "float"},
                            {TokenType::t_identifier, "double"},
                            {TokenType::t_identifier, "char"},
                            {TokenType::t_identifier, "string"},
                            {TokenType::t_identifier, "bool"}});

  std::vector<TypePtr> types{
      BasicType::t_void(),   BasicType::t_int8(),   BasicType::t_int16(),
      BasicType::t_int32(),  BasicType::t_int64(),  BasicType::t_uint8(),
      BasicType::t_uint16(), BasicType::t_uint32(), BasicType::t_uint64(),
      BasicType::t_float(),  BasicType::t_double(), BasicType::t_char(),
      BasicType::t_string(), BasicType::t_bool()};
  for (const auto &type : types) {
    ASSERT_EQ(parser->parseType()->toString(), type->toString());
  }
}

TEST(Parser, compoundTypeParsing) {
  auto parser = makeParser({{TokenType::t_identifier, "a"},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "b"},
                            {TokenType::t_dot},
                            {TokenType::t_identifier, "c"}});

  ASSERT_EQ(parser->parseType()->toString(),
            BasicType::make({"a", "b", "c"})->toString());
}

TEST(Parser, functionTypeParsing) {
  auto parser = makeParser({{TokenType::t_func},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_comma},
                            {TokenType::t_identifier, "float"},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "bool"}});

  ASSERT_EQ(parser->parseType()->toString(),
            FunctionType::make({BasicType::t_int32(), BasicType::t_float()},
                               BasicType::t_bool())
                ->toString());
}

TEST(Parser, pointerTypeParsing) {
  auto parser = makeParser({{TokenType::t_lesser_than},
                            {TokenType::t_greater_than},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_close_bracket},
                            {TokenType::t_identifier, "b"}});

  std::vector<TypePtr> types{
      BasicType::make({"a"}, TypeProperties::_isPointer),
      BasicType::make({"b"},
                      BitField<TypeProperties>{TypeProperties::_isPointer} |
                          TypeProperties::_isShared)};
  for (const auto &type : types) {
    ASSERT_EQ(parser->parseType()->toString(), type->toString());
  }
}

TEST(Parser, containerTypeParsing) {
  // TODO: implement when container types are a things
}

TEST(Parser, typeModifiersParsing) {
  auto parser = makeParser({{TokenType::t_identifier, "a"},
                            {TokenType::t_colon},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_ampersand},
                            {TokenType::t_identifier, "a"}});

  std::vector<TypePtr> types{BasicType::make({"a"}),
                             BasicType::make({"a"}, TypeProperties::_owning),
                             BasicType::make({"a"}, TypeProperties::_mutable)};
  for (const auto &type : types) {
    ASSERT_EQ(parser->parseType()->toString(), type->toString());
  }
}

TEST(Parser, functionTypeMissingComma) {
  std::vector<TokenValuePair> tokens;

  auto parser = makeParser("TEST::Parser.functionTypeMissingComma",
                           {{{0, TokenType::t_func, 0, 4}},
                            {{1, TokenType::t_open_paren, 5, 1}},
                            {{2, TokenType::t_identifier, 6, 3}, "i32"},
                            {{3, TokenType::t_identifier, 10, 5}, "float"},
                            {{4, TokenType::t_close_paren, 16, 1}}},
                           {"func (i32 float)"});

  ASSERT_EQ(parser->parseType(), nullptr);
  ASSERT_EQ(ErrorManager::getErrorType(), ErrorTypes::E_BAD_TOKEN);
}

TEST(Parser, typeEquality) {
  Type::cleanNonBuiltinTypes();
  ASSERT_EQ(BasicType::t_int32(), BasicType::t_int32());
  ASSERT_EQ(BasicType::t_int32(), BasicType::make({"i32"}));
  ASSERT_EQ(BasicType::make({"a"}), BasicType::make({"a"}));
  ASSERT_EQ(BasicType::t_int32()->getMutableType()->getUniquePointerType(),
            BasicType::make(
                {"i32"}, BitField<TypeProperties>{TypeProperties::_isPointer} |
                             TypeProperties::_mutable));
  std::cout << Type::typeBankToString() << std::endl;
}

TEST(Paresr, typeInequality) {
  Type::cleanNonBuiltinTypes();
  ASSERT_NE(BasicType::t_int32(), BasicType::t_double());
  ASSERT_NE(BasicType::make({"a"}), BasicType::make({"b"}));
  ASSERT_NE(BasicType::t_bool(), BasicType::t_bool()->getSharedPointerType());
  std::cout << Type::typeBankToString() << std::endl;
}
