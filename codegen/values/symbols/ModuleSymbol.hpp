#pragma once
#include "codegen/values/symbols/SymbolMap.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

namespace rgl {
class ModuleSymbol;
using ModuleSymbolPtr = std::shared_ptr<ModuleSymbol>;

class ModuleSymbol : public SymbolBase {
public:
  static ModuleSymbolPtr make(const std::vector<std::string> &name,
                              std::shared_ptr<llvm::LLVMContext> context);

  virtual bool isModule() const override { return true; }

  std::vector<std::string> getName() { return m_name; }

  llvm::IRBuilder<> *builder() { return m_builder.get(); }
  SymbolMap &symbols() { return m_symbols; }

private:
  std::vector<std::string> m_name;
  std::shared_ptr<llvm::IRBuilder<>> m_builder;
  SymbolMap m_symbols;

  ModuleSymbol(const std::vector<std::string> &name, llvm::Module *module,
               std::shared_ptr<llvm::IRBuilder<>> builder);
};
}; // namespace rgl
