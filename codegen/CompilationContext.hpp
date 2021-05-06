#pragma once
#include "codegen/ModuleTable.hpp"
#include "common/Core.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

namespace rgl {
class CompilationContext : public ILoggable {
public:
  CompilationContext(std::shared_ptr<llvm::LLVMContext> context,
                     std::shared_ptr<ModuleTableEntry> module);

  std::shared_ptr<ModuleTableEntry> module() { return m_module; }
  llvm::Module *llvmModule() { return m_module->get(); }
  llvm::IRBuilder<> *builder() { return m_module->builder(); }

  virtual std::string toString() const override;

private:
  std::shared_ptr<llvm::LLVMContext> m_context;
  std::shared_ptr<ModuleTableEntry> m_module;
};
}; // namespace rgl
