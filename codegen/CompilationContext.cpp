#include "codegen/CompilationContext.hpp"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

namespace rgl {
CompilationContext::CompilationContext(
    std::shared_ptr<llvm::LLVMContext> context,
    std::shared_ptr<ModuleTableEntry> module)
    : m_context(context), m_module(module) {
  std::vector<llvm::Type *> mainParams{};
  mainParams.reserve(3);
  auto stringArrayType =
      llvm::Type::getInt8Ty(*m_context)->getPointerTo()->getPointerTo();
  mainParams.push_back(stringArrayType);
  mainParams.push_back(llvm::Type::getInt64Ty(*m_context));
  mainParams.push_back(stringArrayType);

  const bool isVarArg = false;
  auto mainType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*m_context),
                                          mainParams, isVarArg);

  auto mainFunction = llvm::Function::Create(
      mainType, llvm::Function::ExternalLinkage, "main", m_module->get());

  std::vector<std::string> argNames{"argv", "argc", "envp"};
  size_t idx = 0;
  for (auto &arg : mainFunction->args()) {
    arg.setName(argNames[idx++]);
  }

  auto entry = llvm::BasicBlock::Create(*m_context, "entry", mainFunction);
  builder()->SetInsertPoint(entry);
}

std::string CompilationContext::toString() const {
  return "CompilationContext";
}
}; // namespace rgl
