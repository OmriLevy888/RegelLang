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
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/ScopeNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/literals/class-literal/ClassLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/ReturnNode.hpp"
#include <iostream>
#include <memory>

#include "lexer/DummyTokenGenerator.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"
namespace rgl {
std::unique_ptr<Parser>
makeParser(std::vector<TokenValuePair> &&tokens,
           std::shared_ptr<SourceProject> sourceProject) {
  auto tokenGenerator =
      std::make_unique<DummyTokenGenerator>(std::move(tokens), sourceProject);
  auto tokenCollection =
      std::make_unique<TokenCollection>(std::move(tokenGenerator));
  return std::make_unique<Parser>(std::move(tokenCollection));
}

std::unique_ptr<Parser> makeParser(const std::string &testName,
                                   std::vector<TokenValuePair> &&tokens,
                                   std::vector<std::string> &&source,
                                   const size_t firstLineNo) {
  auto project = std::make_shared<SourceProject>(testName);

  SourceFile file{testName};
  file.m_lines.reserve(source.size());
  for (size_t idx = 0; idx < source.size(); idx++) {
    file.m_lines.emplace_back(std::move(source[idx]), tokens,
                              firstLineNo + idx);
  }

  project->addFile(std::move(file));
  return makeParser(std::move(tokens), project);
}
}; // namespace rgl

//#define LLVM_TEST_MAIN

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

  auto parser = makeParser({/* {TokenType::t_class}, */
                            /* {TokenType::t_identifier, "geez"}, */
                            /* {TokenType::t_open_bracket}, */
                            /* {TokenType::t_var}, */
                            /* {TokenType::t_open_square}, */
                            /* {TokenType::t_identifier, "a"}, */
                            /* {TokenType::t_comma}, */
                            /* {TokenType::t_identifier, "b"}, */
                            /* {TokenType::t_comma}, */
                            /* {TokenType::t_identifier, "c"}, */
                            /* {TokenType::t_close_square}, */
                            /* {TokenType::t_colon}, */
                            /* {TokenType::t_identifier, "i32"}, */
                            /* {TokenType::t_semicolon}, */
                            /* {TokenType::t_close_bracket}, */

                            {TokenType::t_func},
                            {TokenType::t_identifier, "bar"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_return},
                            {TokenType::t_int32_literal, 5},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket},

                            {TokenType::t_func},
                            {TokenType::t_identifier, "foo"},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_close_paren},
                            {TokenType::t_arrow},
                            {TokenType::t_identifier, "i32"},
                            {TokenType::t_open_bracket},
                            {TokenType::t_return},
                            {TokenType::t_identifier, "a"},
                            {TokenType::t_plus},
                            {TokenType::t_identifier, "bar"},
                            {TokenType::t_open_paren},
                            {TokenType::t_close_paren},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  auto file = parser->parseFile();
  /* std::cout << file->toString() << std::endl; */
  /* std::cout << Context::module()->toString() << std::endl; */
  file->genCode();
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
