#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "lexer/Token.hpp"
#include "tests/TestsCore.hpp"

#include "lexer/Lexer.hpp"
#include "gtest/gtest.h"
#include <memory>

using namespace rgl;

static Lexer makeLexer(std::string &&source, std::string &&testName) {
  auto ss = std::make_unique<TextSourceStream>(source);
  auto sp = std::make_shared<SourceProject>(testName);
  sp->m_files.emplace_back(testName + ".file0");
  return Lexer(std::move(ss), sp);
}

static void assertIdentifier(const TokenValuePair &tvp,
                             const std::string &identifier) {
  ASSERT_TRUE(tvp == TokenType::t_identifier);
  ASSERT_TRUE(tvp.m_value.has_value());
  ASSERT_TRUE(std::get<std::string>(tvp.m_value.value()) == identifier);
}

static void assertChar(const TokenValuePair &tvp, char value) {
  ASSERT_TRUE(tvp == TokenType::t_char_literal);
  ASSERT_TRUE(tvp.m_value.has_value());
  ASSERT_TRUE(std::get<char>(tvp.m_value.value()) == value);
}

static void assertString(const TokenValuePair &tvp, const std::string &str) {
  ASSERT_TRUE(tvp == TokenType::t_string_literal);
  ASSERT_TRUE(tvp.m_value.has_value());
  ASSERT_TRUE(std::get<std::string>(tvp.m_value.value()) == str);
}

static void assertInt(const TokenValuePair &tvp, TokenType type,
                      int64_t value) {
  ASSERT_TRUE(tvp == type);
  ASSERT_TRUE(tvp.m_value.has_value());
  ASSERT_TRUE(std::get<int64_t>(tvp.m_value.value()) == value);
}

static void assertUint(const TokenValuePair &tvp, TokenType type,
                       uint64_t value) {
  ASSERT_TRUE(tvp == type);
  ASSERT_TRUE(tvp.m_value.has_value());
  ASSERT_TRUE(std::get<uint64_t>(tvp.m_value.value()) == value);
}

static void assertDouble(const TokenValuePair &tvp, double value) {
  ASSERT_TRUE(tvp == TokenType::t_double_literal);
  ASSERT_TRUE(tvp.m_value.has_value());
  ASSERT_TRUE(std::get<double>(tvp.m_value.value()) == value);
}

static void assertFloat(const TokenValuePair &tvp, float value) {
  ASSERT_TRUE(tvp == TokenType::t_float_literal);
  ASSERT_TRUE(tvp.m_value.has_value());
  ASSERT_TRUE(std::get<float>(tvp.m_value.value()) == value);
}

TEST(Lexer, keyword) {
  auto lexer = makeLexer("return yield", "TEST::Lexer.keyword");

  auto first = lexer.getNext();
  ASSERT_TRUE(first == TokenType::t_return);
  ASSERT_FALSE(first == TokenType::t_yield);
  auto second = lexer.getNext();
  ASSERT_TRUE(second == TokenType::t_yield);
  auto third = lexer.getNext();
  ASSERT_TRUE(third == TokenType::t_eof);
  ASSERT_TRUE(third == lexer.getNext());
}

TEST(Lexer, identifier) {
  auto lexer =
      makeLexer("return _return a abcdf32 _id", "TEST::Lexer.idenfitier");

  ASSERT_TRUE(lexer.getNext() == TokenType::t_return);
  assertIdentifier(lexer.getNext(), "_return");
  assertIdentifier(lexer.getNext(), "a");
  assertIdentifier(lexer.getNext(), "abcdf32");
  assertIdentifier(lexer.getNext(), "_id");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}

TEST(Lexer, specialCharacter) {
  auto lexer =
      makeLexer("return hello _ ; {]}", "TEST::Lexer.specialCharacter");

  ASSERT_TRUE(lexer.getNext() == TokenType::t_return);
  assertIdentifier(lexer.getNext(), "hello");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_underscore);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_semicolon);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_open_bracket);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_close_square);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_close_bracket);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}

TEST(Lexer, multipleLines) {
  auto lexer = makeLexer("func foo() => i32 {\n    return buffer[idx];\n}",
                         "TEST::Lexer.idenfitier");

  ASSERT_TRUE(lexer.getNext() == TokenType::t_func);
  assertIdentifier(lexer.getNext(), "foo");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_open_paren);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_close_paren);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_arrow);
  assertIdentifier(lexer.getNext(), "i32");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_open_bracket);

  ASSERT_TRUE(lexer.getNext() == TokenType::t_return);
  assertIdentifier(lexer.getNext(), "buffer");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_open_square);
  assertIdentifier(lexer.getNext(), "idx");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_close_square);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_semicolon);

  ASSERT_TRUE(lexer.getNext() == TokenType::t_close_bracket);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}

TEST(Lexer, operators) {
  auto lexer =
      makeLexer("+ += == != >> <<= & => *= |", "TEST::Lexer.operators");

  ASSERT_TRUE(lexer.getNext() == TokenType::t_plus);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_plus_equal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_equal_equal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_not_equal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_shift_right);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_shift_left_equal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_ampersand);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_arrow);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_asterisk_equal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_pipe);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}

TEST(Lexer, lexCharLiteral) {
  auto lexer = makeLexer("'a' 'b' '\\n'\n   'z' '\\'''\\xAf'",
                         "TEST::Lexer.lexCharLiteral");

  assertChar(lexer.getNext(), 'a');
  assertChar(lexer.getNext(), 'b');
  assertChar(lexer.getNext(), '\n');
  assertChar(lexer.getNext(), 'z');
  assertChar(lexer.getNext(), '\'');
  assertChar(lexer.getNext(), '\xaf');
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}

TEST(Lexer, lexStringLiteral) {
  auto lexer = makeLexer("'a' \"hello\\nworld\"\n \"\\\"\\x41\"",
                         "TEST::Lexer.lexStringLiteral");

  assertChar(lexer.getNext(), 'a');
  assertString(lexer.getNext(), "hello\nworld");
  assertString(lexer.getNext(), "\"\x41");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}

TEST(Lexer, lexIntLiteral) {
  auto lexer =
      makeLexer("10 0b1011 0o017 0x41\n10i 10u 10i8; 0x54u64 0b1011u+\n0b0100",
                "TEST::Lexer.lexIntLiteral");

  assertInt(lexer.getNext(), TokenType::t_int32_literal, 10);
  assertInt(lexer.getNext(), TokenType::t_int32_literal, 0b1011);
  assertInt(lexer.getNext(), TokenType::t_int32_literal, 017);
  assertInt(lexer.getNext(), TokenType::t_int32_literal, 0x41);

  assertInt(lexer.getNext(), TokenType::t_int32_literal, 10);
  assertUint(lexer.getNext(), TokenType::t_uint32_literal, 10);
  assertInt(lexer.getNext(), TokenType::t_int8_literal, 10);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_semicolon);
  assertUint(lexer.getNext(), TokenType::t_uint64_literal, 0x54);
  assertUint(lexer.getNext(), TokenType::t_uint32_literal, 0b1011);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_plus);

  assertInt(lexer.getNext(), TokenType::t_int32_literal, 0b0100);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}

TEST(Lexer, lexRealLiteral) {
  auto lexer = makeLexer(".5 .5f 0f 0.f . .d", "TEST::Lexer.lexRealLiteral");

  assertDouble(lexer.getNext(), 0.5);
  assertFloat(lexer.getNext(), 0.5f);
  assertFloat(lexer.getNext(), 0.0f);
  assertFloat(lexer.getNext(), 0.0f);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_dot);
  assertDouble(lexer.getNext(), 0.0);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}

TEST(Lexer, lexComment) {
  auto lexer = makeLexer("let a = // this is some comment\n5;\nlet b /* this "
                         "is an inline comment */ = 3;\nlet /* this is "
                         "a\nmultiline\ncomment */ c = true;",
                         "TEST::Lexer.lexCommen");

  ASSERT_TRUE(lexer.getNext() == TokenType::t_let);
  assertIdentifier(lexer.getNext(), "a");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_equal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_int32_literal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_semicolon);

  ASSERT_TRUE(lexer.getNext() == TokenType::t_let);
  assertIdentifier(lexer.getNext(), "b");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_equal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_int32_literal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_semicolon);

  ASSERT_TRUE(lexer.getNext() == TokenType::t_let);
  assertIdentifier(lexer.getNext(), "c");
  ASSERT_TRUE(lexer.getNext() == TokenType::t_equal);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_boolean);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_semicolon);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}
