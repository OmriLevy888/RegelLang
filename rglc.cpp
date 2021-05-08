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
#include "parser/ast/constructs/FileNode.hpp"
#include "parser/ast/expressions/BasicIdentifierNode.hpp"
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/CompoundIdentifierNode.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/class-literal/ClassLiteralNode.hpp"
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

  auto returnNode = std::make_unique<ReturnNode>(std::make_unique<BinOpNode>(
      BinOpType::b_plus,
      std::make_unique<IntLiteralNode>(1, BasicType::t_int32()),
      std::make_unique<IntLiteralNode>(2, BasicType::t_int32())));

  std::vector<Statement> body{};
  body.push_back(std::move(returnNode));
  auto blockNode = std::make_unique<BlockNode>(std::move(body));

  std::vector<Parameter> parameters;
  parameters.reserve(3);
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_char()->getSharedPointerType(),
      std::make_unique<BasicIdentifierNode>("argv")));
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_int64(), std::make_unique<BasicIdentifierNode>("argc")));
  parameters.push_back(std::make_unique<ParameterNode>(
      BasicType::t_char()->getSharedPointerType(),
      std::make_unique<CompoundIdentifierNode>(
          std::vector<std::string>{"args", "envp"})));

  auto main = std::make_unique<FunctionLiteralNode>(
      std::make_unique<BasicIdentifierNode>("main"), std::move(parameters),
      BasicType::t_uint64(), std::move(blockNode));

  std::vector<FunctionPtr> functions{};
  functions.push_back(std::move(main));

  auto fileNode = std::make_unique<FileNode>(
      std::make_unique<NamespaceDeclarationNode>(
          std::make_unique<CompoundIdentifierNode>(
              std::vector<std::string>{"my", "module", "funcking", "works"})),
      std::vector<ClassPtr>{}, std::move(functions), std::vector<Expression>{});

  std::cout << Context::module()->toString() << std::endl;
  std::cout << "foo" << std::endl;
  fileNode->genCode();
  std::cout << "bar" << std::endl;
  std::cout << Context::module()->toString() << std::endl;

  return 0;
}
#else
#include "gtest/gtest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
