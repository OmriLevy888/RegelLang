#include <iostream>
#include "common/Core.h"
#include "common/collections/source-stream/FileSourceStream.h"
#include "common/collections/source-stream/TextSourceStream.h"
#include "lexer/TokenCollection.h"
#include "lexer/Lexer.h"
#include "lexer/DummyTokenGenerator.h"
#include "common/Formatter.h"

#ifndef RGL_TESTS
int main(int argc, char **argv, char **envp) {
  rgl::Logger::init();
  rgl::Logger::setPrefixDate(true);
  rgl::Logger::setLogLevel(rgl::LogLevel::debug);

  rgl::Logger::error("Something bad has happened :>");
  std::string a = "hello world\nthis is some\n text";
  rgl::Logger::info(a.find('\n', 0));
  rgl::Logger::info(a.find('\n', 12));
  rgl::Logger::info(a.find('\n', 25) == std::string::npos);
  rgl::Logger::info("Some string");
  rgl::Logger::info(5);
  rgl::Logger::info('A');
  rgl::Logger::info(rgl::Formatter("Format {}", "string"));

  rgl::TextSourceStream tss(
    "This\nis a\ntest to see\nhow the\nTextSourceStream handles multiple lines");
  std::string line;
  while (tss.readLine(line)) {
    rgl::Logger::warning(line);
  }

  rgl::FileSourceStream fss("test.txt");
  while (fss.readLine(line)) {
    rgl::Logger::error(line);
  }

  return 0;
}
#endif