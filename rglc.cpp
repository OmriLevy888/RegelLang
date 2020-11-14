#include <iostream>
#include "common/Core.h"
#include "common/collections/source-stream/FileSourceStream.h"
#include "common/collections/source-stream/TextSourceStream.h"
#include "lexer/TokenCollection.h"
#include "lexer/Lexer.h"
#include "lexer/DummyTokenGenerator.h"

#ifndef RGL_TESTS
using namespace rgl;

int main(int argc, char **argv, char **envp) {
  Logger::init();
  Logger::setPrefixDate(true);
  Logger::setLogLevel(LogLevel::debug);

  Logger::error("Something bad has happened :>");
  std::string a = "hello world\nthis is some\n text";
  Logger::info(a.find('\n', 0));
  Logger::info(a.find('\n', 12));
  Logger::info(a.find('\n', 25) == std::string::npos);
  Logger::info("Some string");
  Logger::info(5);
  Logger::info('A');
  Logger::info(Formatter("Format {}", "string"));

  TextSourceStream tss(
    "This\nis a\ntest to see\nhow the\nTextSourceStream handles multiple lines");
  std::string line;
  while (tss.readLine(line)) {
    Logger::warning(line);
  }

  FileSourceStream fss("test.txt");
  while (fss.readLine(line)) {
    Logger::error(line);
  }

  SourceLine sl("func foo(int a, char b, starng c):");
  sl.m_tokens.emplace_back(TokenType::t_func, 0, std::string_view(sl.m_repr.c_str(), 4));
  sl.m_tokens.emplace_back(TokenType::t_identifier, 1, std::string_view(sl.m_repr.c_str() + 5, 3));
  sl.m_tokens.emplace_back(TokenType::t_open_paren, 2, std::string_view(sl.m_repr.c_str() + 8, 1));
  sl.m_tokens.emplace_back(TokenType::t_identifier, 3, std::string_view(sl.m_repr.c_str() + 9, 3));
  sl.m_tokens.emplace_back(TokenType::t_identifier, 4, std::string_view(sl.m_repr.c_str() + 13, 1));
  sl.m_tokens.emplace_back(TokenType::t_comma, 5, std::string_view(sl.m_repr.c_str() + 14, 1));
  sl.m_tokens.emplace_back(TokenType::t_identifier, 6, std::string_view(sl.m_repr.c_str() + 16, 4));
  sl.m_tokens.emplace_back(TokenType::t_identifier, 7, std::string_view(sl.m_repr.c_str() + 21, 1));
  sl.m_tokens.emplace_back(TokenType::t_comma, 8, std::string_view(sl.m_repr.c_str() + 22, 1));
  sl.m_tokens.emplace_back(TokenType::t_identifier, 9, std::string_view(sl.m_repr.c_str() + 24, 6));
  sl.m_tokens.emplace_back(TokenType::t_identifier, 10, std::string_view(sl.m_repr.c_str() + 31, 1));
  sl.m_tokens.emplace_back(TokenType::t_close_paren, 11, std::string_view(sl.m_repr.c_str() + 32, 1));
  sl.m_tokens.emplace_back(TokenType::t_colon, 12, std::string_view(sl.m_repr.c_str() + 33, 1));
  std::cout << sl.pointAt(8) << std::endl;
  std::cout << sl.pointAt(9) << std::endl;
  std::cout << sl.pointAt(10) << std::endl;
  for (const auto &token : sl.m_tokens) {
    std::cout << token.m_repr << std::endl;
  }
  return 0;
}
#endif