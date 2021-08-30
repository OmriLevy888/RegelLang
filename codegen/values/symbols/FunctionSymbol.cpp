#include "codegen/values/symbols/FunctionSymbol.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/symbols/types/FunctionTypeSymbol.hpp"
#include "common/Formatter.hpp"
#include "llvm/Support/raw_ostream.h"

namespace rgl {
FunctionSymbol::FunctionSymbol(
    const std::vector<std::string> &name,
    const std::vector<std::vector<std::string>> &paramNames,
    FunctionTypeSymbolPtr type, llvm::Function *llvmFunction)
    : CallableSymbol(llvmFunction), m_name(name), m_paramNames(paramNames),
      m_type(type) {}

FunctionSymbolPtr FunctionSymbol::make(const std::vector<std::string> &name,
                                       TypeSymbolPtr retType,
                                       const std::vector<Parameter> &parameters,
                                       bool isVarArg) {
  std::vector<TypeSymbolPtr> paramTypes{};
  paramTypes.reserve(parameters.size());
  for (const auto &param : parameters) {
    paramTypes.push_back(
        Context::module()->symbols().getType(param->getType()));
  }
  auto functionTypeSymbol = Context::module()->symbols().getFunctionType(
      retType, std::move(paramTypes));
  auto llvmFunctionType =
      reinterpret_cast<llvm::FunctionType *>(functionTypeSymbol->llvmType());

  const std::string functionName = Formatter(
      "{}::{}", Formatter<>::joinContainer('.', Context::module()->getName()),
      Formatter<>::joinContainer('.', name));
  llvm::Function *llvmFunction =
      llvm::Function::Create(llvmFunctionType, llvm::Function::ExternalLinkage,
                             functionName, Context::llvmModule());

  size_t idx = 0;
  for (auto &arg : llvmFunction->args()) {
    arg.setName(parameters[idx++]->getNameString());
  }

  std::vector<std::vector<std::string>> paramNames{};
  paramNames.reserve(parameters.size());
  for (const auto &param : parameters) {
    paramNames.push_back(param->getName());
  }

  return FunctionSymbolPtr(
      new FunctionSymbol(name, paramNames, functionTypeSymbol, llvmFunction));
}

void FunctionSymbol::genCode(const Expression &body,
                             FunctionSymbolPtr thisSharedPtr) {
  auto functionSymbol = thisSharedPtr;
  if (nullptr == functionSymbol) {
    auto symbol = Context::module()->symbols().get(m_name);
    if (nullptr == symbol || !symbol->isFunction()) {
      // TODO: propagate error
    }
    functionSymbol = std::dynamic_pointer_cast<FunctionSymbol>(symbol);
  }

  auto llvmFunction =
      reinterpret_cast<llvm::Function *>(functionSymbol->llvmValue());
  auto entry =
      llvm::BasicBlock::Create(*Context::llvmContext(), "entry", llvmFunction);
  Context::builder()->SetInsertPoint(entry);

  // Create stack frame
  Context::getCurrContext()->pushGeneratedFunction(functionSymbol);
  auto functionTopLevelStackFrame = functionSymbol->createStackFrame();

  for (size_t idx = 0; idx < m_paramNames.size(); idx++) {
    functionTopLevelStackFrame->createParameter(m_paramNames[idx],
                                                m_type->paramTypes()[idx],
                                                llvmFunction->getArg(idx));
  }

  body->genCode();
  functionSymbol->removeStackFrame();
  Context::getCurrContext()->popGeneratedFunction();
}

SymbolMapPtr FunctionSymbol::createStackFrame() {
  auto stackFrame = std::make_shared<SymbolMap>();
  m_stackFrames.push_back(stackFrame);
  return stackFrame;
}
void FunctionSymbol::removeStackFrame() {
  m_stackFrames[m_stackFrames.size() - 1]->clean();
  m_stackFrames.pop_back();
}
SymbolMapPtr FunctionSymbol::getCurrStackFrame() {
  return m_stackFrames[m_stackFrames.size() - 1];
}
std::vector<SymbolMapPtr> &FunctionSymbol::getStackFrames() {
  return m_stackFrames;
}
}; // namespace rgl
