#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
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

TEST(Lexer, keyword) {
  auto lexer = makeLexer("return yield", "TEST::Lexer.keyword");

  Token first = lexer.getNext();
  ASSERT_TRUE(first == TokenType::t_return);
  ASSERT_FALSE(first == TokenType::t_yield);
  Token second = lexer.getNext();
  ASSERT_TRUE(second == TokenType::t_yield);
  Token third = lexer.getNext();
  ASSERT_TRUE(third == TokenType::t_eof);
  ASSERT_TRUE(third == lexer.getNext());
}

TEST(Lexer, identifier) {
  auto lexer =
      makeLexer("return _return a abcdf32 _id", "TEST::Lexer.idenfitier");

  ASSERT_TRUE(lexer.getNext() == TokenType::t_return);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_eof);
}

TEST(Lexer, specialCharacter) {
  auto lexer =
      makeLexer("return hello _ ; {]}", "TEST::Lexer.specialCharacter");

  ASSERT_TRUE(lexer.getNext() == TokenType::t_return);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
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
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_open_paren);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_close_paren);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_arrow);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_open_bracket);

  ASSERT_TRUE(lexer.getNext() == TokenType::t_return);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_open_square);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
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
