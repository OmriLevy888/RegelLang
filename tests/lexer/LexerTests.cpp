#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
#include "lexer/Token.hpp"
#include "tests/TestsCore.hpp"

#include "lexer/Lexer.hpp"
#include "gtest/gtest.h"
#include <memory>

using namespace rgl;

TEST(Lexer, keyword) {
  auto ss = std::make_unique<rgl::TextSourceStream>("return yield");
  auto sp = std::make_shared<rgl::SourceProject>("TEST::Lexer.keyword");
  sp->m_files.emplace_back("TEST::Lexer.keyword.file0");
  Lexer lexer{std::move(ss), sp};

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
  auto ss =
      std::make_unique<rgl::TextSourceStream>("return _return a abcdf32 _id");
  auto sp = std::make_shared<rgl::SourceProject>("TEST::Lexer.identifier");
  sp->m_files.emplace_back("TEST::Lexer.identifier.file0");
  Lexer lexer{std::move(ss), sp};

  ASSERT_TRUE(lexer.getNext() == TokenType::t_return);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
}

TEST(Lexer, specialCharacter) {
  auto ss = std::make_unique<rgl::TextSourceStream>("return hello _ ; {]}");
  auto sp =
      std::make_shared<rgl::SourceProject>("TEST::Lexer.specialCharacter");
  sp->m_files.emplace_back("TEST::Lexer.specialCharacter.file0");
  Lexer lexer{std::move(ss), sp};

  ASSERT_TRUE(lexer.getNext() == TokenType::t_return);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_identifier);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_underscore);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_semicolon);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_open_bracket);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_close_square);
  ASSERT_TRUE(lexer.getNext() == TokenType::t_close_bracket);
}
