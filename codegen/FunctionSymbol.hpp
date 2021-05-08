#pragma once
#include "codegen/SymbolBase.hpp"
#include "parser/ast/constructs/FunctionType.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"
#include "llvm/IR/Function.h"

namespace rgl {
class FunctionSymbol;
using FunctionSymbolPtr = std::shared_ptr<FunctionSymbol>;

class FunctionSymbol : public ILoggable {
public:
  static FunctionSymbolPtr make(const std::vector<std::string> &name,
                                TypePtr retType,
                                const std::vector<Parameter> &parameters,
                                bool isVarArg = false);

  llvm::Function *llvmFunction() { return m_llvmFunction; }

  virtual std::string toString() const override;

private:
  FunctionTypePtr m_type;
  llvm::Function *m_llvmFunction;

  FunctionSymbol(FunctionTypePtr type, llvm::Function *llvmFunction);
};
}; // namespace rgl
