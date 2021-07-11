#pragma once
#include "codegen/values/symbols/SymbolBase.hpp"
#include "codegen/values/symbols/SymbolMap.hpp"
#include "parser/ast/constructs/FunctionType.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"

namespace rgl {
class FunctionSymbol;
using FunctionSymbolPtr = std::shared_ptr<FunctionSymbol>;

class FunctionSymbol : public SymbolBase {
public:
  static FunctionSymbolPtr make(const std::vector<std::string> &name,
                                TypePtr retType,
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

private:
  std::vector<std::string> m_name;
  std::vector<std::vector<std::string>> m_paramNames;
  FunctionTypePtr m_type;
  std::vector<SymbolMapPtr> m_stackFrames;

  FunctionSymbol(const std::vector<std::string> &name,
                 const std::vector<std::vector<std::string>> &paramNames,
                 FunctionTypePtr type, llvm::Function *llvmFunction);
};
}; // namespace rgl
