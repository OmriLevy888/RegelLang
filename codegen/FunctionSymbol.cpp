#include "codegen/FunctionSymbol.hpp"
#include "codegen/Context.hpp"
#include "common/Formatter.hpp"
#include "llvm/Support/raw_ostream.h"

namespace rgl {
FunctionSymbol::FunctionSymbol(
    const std::vector<std::string> &name,
    const std::vector<std::vector<std::string>> &paramNames,
    FunctionTypePtr type, llvm::Function *llvmFunction)
    : m_name(name), m_paramNames(paramNames), m_type(type),
      m_llvmFunction(llvmFunction) {}

FunctionSymbolPtr FunctionSymbol::make(const std::vector<std::string> &name,
                                       TypePtr retType,
                                       const std::vector<Parameter> &parameters,
                                       bool isVarArg) {
  std::vector<TypePtr> paramTypes{};
  paramTypes.reserve(parameters.size());
  for (const auto &param : parameters) {
    paramTypes.push_back(param->getType());
  }
  auto functionType = FunctionType::make(std::move(paramTypes), retType);
  auto llvmFunctionType = functionType->toLLVMType();

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
      new FunctionSymbol(name, paramNames, functionType, llvmFunction));
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

  auto llvmFunction = functionSymbol->llvmFunction();
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

std::string FunctionSymbol::toString() const {
  std::string str;
  llvm::raw_string_ostream rso{str};
  rso << *m_llvmFunction;
  rso.flush();
  return Formatter("FunctionSymbol<{}>", std::move(str));
}
}; // namespace rgl
