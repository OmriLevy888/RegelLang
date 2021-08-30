#pragma once
#include "codegen/values/symbols/CallableSymbol.hpp"
#include "codegen/values/symbols/SymbolMap.hpp"
#include "codegen/values/symbols/types/TypeSymbolBase.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"

namespace rgl {
class FunctionSymbol;
using FunctionSymbolPtr = std::shared_ptr<FunctionSymbol>;

class FunctionSymbol : public CallableSymbol {
public:
  static FunctionSymbolPtr make(const std::vector<std::string> &name,
                                TypeSymbolPtr retType,
                                const std::vector<Parameter> &parameters,
                                bool isVarArg = false);

  virtual bool isFunction() const override { return true; }

  // If thisSharedPtr is not passed, the function will look it up
  void genCode(const Expression &body,
               FunctionSymbolPtr thisSharedPtr = nullptr);

  SymbolMapPtr createStackFrame();
  void removeStackFrame();
  SymbolMapPtr getCurrStackFrame();
  // The top stack frames is at index [.size() - 1]
  std::vector<SymbolMapPtr> &getStackFrames();

  FunctionTypeSymbolPtr type() const { return m_type; }

private:
  std::vector<std::string> m_name;
  std::vector<std::vector<std::string>> m_paramNames;
  FunctionTypeSymbolPtr m_type;
  std::vector<SymbolMapPtr> m_stackFrames;

  FunctionSymbol(const std::vector<std::string> &name,
                 const std::vector<std::vector<std::string>> &paramNames,
                 FunctionTypeSymbolPtr type, llvm::Function *llvmFunction);
};
}; // namespace rgl
