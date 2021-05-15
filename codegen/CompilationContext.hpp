#pragma once
#include "codegen/FunctionSymbol.hpp"
#include "codegen/ModuleSymbol.hpp"
#include "common/Core.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <stack>

namespace rgl {
class CompilationContext : public ILoggable {
public:
  CompilationContext(std::shared_ptr<llvm::LLVMContext> context,
                     ModuleSymbolPtr module);

  ModuleSymbolPtr module() { return m_module; }
  llvm::Module *llvmModule() { return m_module->get(); }
  llvm::IRBuilder<> *builder() { return m_module->builder(); }
  FunctionSymbolPtr getCurrGeneratedFunction() {
    return m_generatingFunctions.top();
  }
  void pushGeneratedFunction(FunctionSymbolPtr function) {
    m_generatingFunctions.push(function);
  }
  void popGeneratedFunction() { m_generatingFunctions.pop(); }

  virtual std::string toString() const override;

private:
  std::shared_ptr<llvm::LLVMContext> m_context;
  ModuleSymbolPtr m_module;
  std::stack<FunctionSymbolPtr> m_generatingFunctions;
};
}; // namespace rgl
