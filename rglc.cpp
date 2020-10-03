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

  rgl::FileSourceStream fss("bin/test.bin");
  rgl::Logger::debug(fss);
  rgl::Logger::critical(fss);
  rgl::Logger::setLogLevel(rgl::LogLevel::error);
  rgl::Logger::debug(fss);

  rgl::TextSourceStream tss("This is some source");
  tss.seek(2);
  rgl::Logger::error(tss);

  rgl::Lexer lexer(std::make_unique<rgl::TextSourceStream>("This is some source code"));
  rgl::Logger::init();
  rgl::Logger::info(lexer);

  rgl::TokenCollection tc(std::make_unique<rgl::DummyTokenGenerator>(std::vector<rgl::Token>
    { rgl::Token{false},
      rgl::Token{false},
      rgl::Token{false},
      rgl::Token{false},
      rgl::Token{true}, 
      rgl::Token{false},
      rgl::Token{true}}));
  rgl::Logger::info(tc);

  rgl::Logger::warning(rgl::Formatter("{1} + {0} = {3}", 1, 2, 3, tc));

  if (!fss.isOpen()) {
    std::cout << "Failed to open fss" << std::endl;
  }
  else {
    for (int curr = fss.read(); !fss.eof(); curr = fss.read()) {
      std::cout << (char)curr;
    }
  }

  return 0;
}
#endif