#pragma once
#include "codegen/SymbolBase.hpp"
#include "codegen/SymbolMap.hpp"
#include "parser/ast/constructs/FunctionType.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"
#include "llvm/IR/Function.h"

namespace rgl {
class FunctionSymbol;
using FunctionSymbolPtr = std::shared_ptr<FunctionSymbol>;

class FunctionSymbol : public SymbolBase {
public:
  static FunctionSymbolPtr make(const std::vector<std::string> &name,
                                TypePtr retType,
                                const std::vector<Parameter> &parameters,
                                bool isVarArg = false);

  llvm::Function *llvmFunction() { return m_llvmFunction; }

  SymbolMapPtr createStackFrame();
  void removeStackFrame();
  SymbolMapPtr getCurrStackFrame();
  // The top stack frames is at index [.size() - 1]
  std::vector<SymbolMapPtr> &getStackFrames();

  virtual std::string toString() const override;

private:
  FunctionTypePtr m_type;
  llvm::Function *m_llvmFunction;
  std::vector<SymbolMapPtr> m_stackFrames;

  FunctionSymbol(FunctionTypePtr type, llvm::Function *llvmFunction);
};
}; // namespace rgl
