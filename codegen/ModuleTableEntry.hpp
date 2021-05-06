#pragma once
#include "common/ILoggable.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

namespace rgl {
class ModuleTableEntry : public ILoggable {
public:
  ModuleTableEntry(const std::string &name,
                   std::shared_ptr<llvm::LLVMContext> context);

  llvm::Module *get() { return m_module.get(); }
  llvm::IRBuilder<> *builder() { return m_builder.get(); }

  virtual std::string toString() const override;

private:
  std::shared_ptr<llvm::Module> m_module;
  std::shared_ptr<llvm::IRBuilder<>> m_builder;
};
}; // namespace rgl
