#include "cli/CliParser.hpp"
#include "cli/Context.hpp"
#include "cli/ProjectFileParser.hpp"
#include "common/Core.hpp"
#include "common/collections/source-objects/SourceFile.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/FileSourceStream.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
#include "common/errors/ErrorManager.hpp"
#include "lexer/DummyTokenGenerator.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/TokenCollection.hpp"
#include <iostream>
#include <memory>

#ifndef RGL_TESTS
using namespace rgl;

int main(int argc, const char **argv, char **envp) {
  if (!CliParser::parseCliArgument(argc, argv)) {
    return -1;
  }

  Logger::init();
  Logger::setPrefixDate(true);
  Logger::setLogLevel(LogLevel::debug);
  Context &context = Context::getInstance();

  if (!ProjectFileParser::parseProjectFile(
          context.m_cliArguments.m_projectFilePath,
          context.m_cliArguments.m_target)) {
    ErrorManager::logError("Failed to parse project file");
    return -1;
  }

  auto project = std::make_shared<SourceProject>("Project");
  for (const auto &file : context.m_target.m_files) {
    std::cout << "working on file " << file << std::endl;
    size_t fileIdx = project->addFile(SourceFile{file});
    auto fss = std::make_unique<FileSourceStream>(file, fileIdx);
    Lexer lexer{std::move(fss), project};
    auto [curr, _] = lexer.getNext();
    do {
      std::cout << curr.toString() << std::endl;
      curr = lexer.getNext().m_token;
    } while (curr != TokenType::t_eof);
    std::cout << std::endl;
  }

  return 0;
}
#endif
