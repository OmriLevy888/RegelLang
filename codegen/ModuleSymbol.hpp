#pragma once
#include "codegen/SymbolMap.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <string>

namespace rgl {
class ModuleSymbol;
using ModuleSymbolPtr = std::shared_ptr<ModuleSymbol>;

class ModuleSymbol : public SymbolBase {
public:
  static ModuleSymbolPtr make(const std::vector<std::string> &name,
                              std::shared_ptr<llvm::LLVMContext> context);

  std::vector<std::string> getName() { return m_name; }

  llvm::Module *get() { return m_module.get(); }
  llvm::IRBuilder<> *builder() { return m_builder.get(); }
  SymbolMap &symbols() { return m_symbols; }

  virtual std::string toString() const override;

private:
  std::vector<std::string> m_name;
  std::shared_ptr<llvm::Module> m_module;
  std::shared_ptr<llvm::IRBuilder<>> m_builder;
  SymbolMap m_symbols;

  ModuleSymbol(const std::vector<std::string> &name,
               std::shared_ptr<llvm::Module> module,
               std::shared_ptr<llvm::IRBuilder<>> builder);
};
}; // namespace rgl
