#include "codegen/FunctionSymbol.hpp"
#include "codegen/Context.hpp"
#include "common/Formatter.hpp"
#include "llvm/Support/raw_ostream.h"

namespace rgl {
FunctionSymbol::FunctionSymbol(FunctionTypePtr type,
                               llvm::Function *llvmFunction)
    : m_type(type), m_llvmFunction(llvmFunction) {}

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

  std::vector<std::string> paramNames{};
  paramNames.reserve(parameters.size());
  for (const auto &param : parameters) {
    paramNames.push_back(Formatter<>::joinContainer('.', param->getName()));
  }
  size_t idx = 0;
  for (auto &arg : llvmFunction->args()) {
    arg.setName(paramNames[idx++]);
  }

  return FunctionSymbolPtr(new FunctionSymbol(functionType, llvmFunction));
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
