#include "cli/ProjectFileParser.hpp"
#include "common/Core.hpp"
#include "common/collections/source-objects/SourceFile.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/FileSourceStream.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
#include "lexer/DummyTokenGenerator.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/TokenCollection.hpp"
#include <iostream>
#include <memory>

#ifndef RGL_TESTS
using namespace rgl;

int main(int argc, char **argv, char **envp) {
  Logger::init();
  Logger::setPrefixDate(true);
  Logger::setLogLevel(LogLevel::debug);

  /*if (argc == 1) {
    Logger::critical("Usage: ", argv[0], "<File[ File[ File[ ... ]]]>");
    return -1;
  }

  std::vector<std::string> files;
  files.reserve(argc - 1);
  for (size_t idx = 1; idx < argc; idx++) {
    files.push_back(std::string(argv[idx]));
  }*/
  if (!ProjectFileParser::parseProjectFile("../source/sample.json",
                                           "Release")) {
    std::cout << "Failed to parse project file" << std::endl;
    return -1;
  }

  auto project = std::make_shared<SourceProject>("Project");
  for (const auto &file : Context::getInstance().m_target.m_files) {
    std::cout << "working on file " << file << std::endl;
    size_t fileIdx = project->addFile(SourceFile{file});
    auto fss = std::make_unique<FileSourceStream>(file, fileIdx);
    Lexer lexer{std::move(fss), project};
    Token curr = lexer.getNext();
    do {
      std::cout << curr.toString() << std::endl;
      curr = lexer.getNext();
    } while (curr != TokenType::t_eof);
    std::cout << std::endl;
  }

  return 0;
}
#endif
