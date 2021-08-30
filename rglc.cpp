#include "cli/CLIContext.hpp"
#include "cli/CLIParser.hpp"
#include "cli/ProjectFileParser.hpp"
#include "codegen/Context.hpp"
#include "common/Core.hpp"
#include "common/errors/ErrorManager.hpp"
#include "common/source-objects/SourceProject.hpp"
#include "common/source-stream/FileSourceStream.hpp"
#include "common/source-stream/TextSourceStream.hpp"
#include "lexer/DummyTokenGenerator.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/TokenCollection.hpp"
#include "parser/Parser.hpp"
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

//#define LLVM_TEST_MAIN

#ifndef RGL_TESTS
#include "lexer/DummyTokenGenerator.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

namespace rgl {
std::unique_ptr<Parser> makeParser(std::vector<TokenValuePair> &&tokens) {
  auto tokenGenerator =
      std::make_unique<DummyTokenGenerator>(std::move(tokens));
  auto tokenCollection =
      std::make_unique<TokenCollection>(std::move(tokenGenerator));
  return std::make_unique<Parser>(std::move(tokenCollection));
}

std::unique_ptr<Parser> makeParser(const std::string &testName,
                                   std::vector<TokenValuePair> &&tokens,
                                   std::vector<std::string> &&source,
                                    size_t firstLineNo) {
  SourceProject::clean();
  SourceProject::get().name() = testName;

  SourceFile file{testName};
  file.m_lines.reserve(source.size());
  for (size_t idx = 0; idx < source.size(); idx++) {
    file.m_lines.emplace_back(std::move(source[idx]), tokens,
                              firstLineNo + idx);
  }

  SourceProject::get().addFile(std::move(file));
  return makeParser(std::move(tokens));
}
}; // namespace rgl

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

  auto parser = makeParser(
      "GENERIC-TEST",
      {/* {TokenType::t_class}, */
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

       {{TokenType::t_func, {0, 0, 0, 4}}},
       {{TokenType::t_identifier, {0, 0, 5, 3}}, "bar"},
       {{TokenType::t_open_paren, {0, 0, 8, 1}}},
       {{TokenType::t_close_paren, {0, 0, 9, 1}}},
       {{TokenType::t_arrow, {0, 0, 11, 2}}},
       {{TokenType::t_identifier, {0, 0, 14, 3}}, "i32"},
       {{TokenType::t_open_bracket, {0, 0, 18, 1}}},
       {{TokenType::t_return, {0, 1, 4, 6}}},
       {{TokenType::t_int32_literal, {0, 1, 11, 1}}, 5},
       {{TokenType::t_semicolon, {0, 1, 12, 1}}},
       {{TokenType::t_close_bracket, {0, 2, 0, 1}}},

       {{TokenType::t_func, {0, 4, 0, 4}}},
       {{TokenType::t_identifier, {0, 4, 5, 3}}, "foo"},
       {{TokenType::t_open_paren, {0, 4, 8, 1}}},
       {{TokenType::t_identifier, {0, 4, 9, 3}}, "i32"},
       {{TokenType::t_identifier, {0, 4, 13, 1}}, "a"},
       {{TokenType::t_close_paren, {0, 4, 14, 1}}},
       {{TokenType::t_arrow, {0, 4, 16, 2}}},
       {{TokenType::t_identifier, {0, 4, 19, 3}}, "i32"},
       {{TokenType::t_open_bracket, {0, 4, 23, 1}}},
       {{TokenType::t_return, {0, 5, 4, 6}}},
       {{TokenType::t_identifier, {0, 5, 11, 1}}, "a"},
       {{TokenType::t_plus, {0, 5, 13, 1}}},
       {{TokenType::t_identifier, {0, 5, 15, 3}}, "bar"},
       {{TokenType::t_open_paren, {0, 5, 19, 1}}},
       {{TokenType::t_close_paren, {0, 5, 20, 1}}},
       {{TokenType::t_semicolon, {0, 5, 21, 1}}},
       {{TokenType::t_close_bracket, {0, 6, 0, 1}}}},

      {"func bar() => i32 {", "    return 5;", "}", "",
       "func foo(i32 a) => i32 {", "    return a + bar();", "}"});

  ErrorManager::setDisplayStackTrace(true);
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
