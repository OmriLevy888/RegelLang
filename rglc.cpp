#include "cli/CLIContext.hpp"
#include "cli/CLIParser.hpp"
#include "cli/ProjectFileParser.hpp"
#include "codegen/Context.hpp"
#include "common/Core.hpp"
#include "common/collections/source-objects/SourceFile.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/FileSourceStream.hpp"
#include "common/collections/source-stream/TextSourceStream.hpp"
#include "common/errors/ErrorManager.hpp"
#include "lexer/DummyTokenGenerator.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/TokenCollection.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/statements/ReturnNode.hpp"
#include <iostream>
#include <memory>

#ifndef RGL_TESTS
using namespace rgl;

int main(int argc, const char **argv, char **envp) {
  /* if (!CliParser::parseCliArgument(argc, argv)) { */
  /*   return -1; */
  /* } */

  /* Logger::init(); */
  /* Logger::setPrefixDate(true); */
  /* Logger::setLogLevel(LogLevel::debug); */
  /* Context &context = Context::getInstance(); */

  /* if (!ProjectFileParser::parseProjectFile( */
  /*         context.m_cliArguments.m_projectFilePath, */
  /*         context.m_cliArguments.m_target)) { */
  /*   ErrorManager::logError("Failed to parse project file"); */
  /*   return -1; */
  /* } */

  auto expr = std::make_unique<ReturnNode>(std::make_unique<BinOpNode>(
      BinOpType::b_plus,
      std::make_unique<IntLiteralNode>(1, BasicType::t_int32()),
      std::make_unique<IntLiteralNode>(2, BasicType::t_int32())));

  std::cout << expr->toString() << std::endl;

  std::cout << Context::getCurrContext()->module()->toString() << std::endl;
  expr->genCode();
  std::cout << Context::getCurrContext()->module()->toString() << std::endl;

  return 0;
}
#else
#include "gtest/gtest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
