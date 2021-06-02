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
using namespace rgl;

int main(int argc, const char **argv, char **envp) {
#ifdef LLVM_TEST_MAIN
  auto context = std::make_shared<llvm::LLVMContext>();
  auto module = std::make_shared<llvm::Module>("TestModule", *context);
  auto builder = std::make_shared<llvm::IRBuilder<>>(*context);
  auto printModule = [module]() {
    std::string str{};
    llvm::raw_string_ostream rso{str};
    rso << *module;
    rso.flush();
    std::cout << str << std::endl;
  };
  printModule();

  auto fooType = llvm::FunctionType::get(
      llvm::Type::getInt32Ty(*context),
      std::vector<llvm::Type *>{llvm::Type::getInt32Ty(*context)}, false);
  auto fooFunc = llvm::Function::Create(
      fooType, llvm::Function::ExternalLinkage, "foo", *module);
  auto fooEntryBB = llvm::BasicBlock::Create(*context, "entry", fooFunc);
  builder->SetInsertPoint(fooEntryBB);
  auto callInstr = builder->CreateCall(
      fooFunc, llvm::ArrayRef<llvm::Value *>{
                   llvm::ConstantInt::get(*context, llvm::APInt(32, 5, true)),
                   llvm::ConstantInt::get(*context, llvm::APInt(32, 0, true))});
  printModule();

  auto barType = llvm::FunctionType::get(
      llvm::Type::getVoidTy(*context),
      std::vector<llvm::Type *>{llvm::Type::getInt32Ty(*context),
                                llvm::Type::getInt32Ty(*context)},
      false);
  auto barFunc = llvm::Function::Create(
      barType, llvm::Function::ExternalLinkage, "bar", *module);
  // callInstr->setCalledFunction(barFunc); // does not work with different
  // return type
  callInstr->setCalledOperand(barFunc); // works with different
  callInstr->setName("");
  auto barCall = builder->CreateCall(
      barFunc, llvm::ArrayRef<llvm::Value *>{
                   llvm::ConstantInt::get(*context, llvm::APInt(32, 2, true))});
  // return type(?)
  // callInstr->setCalledFunction(barType, barFunc); // does not work with
  // different return type
  printModule();

  // barCall->setCalledOperand(fooFunc);
  barCall->setName("foo");
  barCall->setCalledFunction(fooType, fooFunc);
  printModule();
#else
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

  auto fooVarDecl =
      std::make_unique<ExpressionStatementNode>(std::make_unique<VarDeclNode>(
          std::make_unique<BasicIdentifierNode>("kraa"), BasicType::t_int32(),
          std::make_unique<IntLiteralNode>(5ull, BasicType::t_int32())));
  auto fooRet = std::make_unique<ReturnNode>(
      std::make_unique<BasicIdentifierNode>("kraa"));
  std::vector<Statement> fooStatements{};
  fooStatements.push_back(std::move(fooVarDecl));
  fooStatements.push_back(std::move(fooRet));
  auto fooBody = std::make_unique<BlockNode>(std::move(fooStatements));

  auto fooFunc = std::make_unique<FunctionLiteralNode>(
      std::make_unique<CompoundIdentifierNode>(
          std::vector<std::string>{"foo", "func"}),
      std::vector<Parameter>{}, BasicType::t_int32(), std::move(fooBody));

  std::vector<Statement> fileStatements{};
  fileStatements.push_back(
      std::make_unique<ExpressionStatementNode>(std::move(main)));
  fileStatements.push_back(
      std::make_unique<ExpressionStatementNode>(std::move(fooFunc)));
  auto fileBody = std::make_unique<BlockNode>(std::move(fileStatements));

  auto fileNode = std::make_unique<FileNode>(
      std::make_unique<NamespaceDeclarationNode>(
          std::make_unique<CompoundIdentifierNode>(
              std::vector<std::string>{"my", "module", "funcking", "works"})),
      std::move(fileBody));

  std::cout << fileNode->toString() << std::endl;

  std::cout << Context::module()->toString() << std::endl;
  fileNode->genCode();
  std::cout << Context::module()->toString() << std::endl;
#endif

  return 0;
}
#else
#include "gtest/gtest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
