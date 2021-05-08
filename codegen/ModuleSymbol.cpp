#include "codegen/ModuleSymbol.hpp"
#include "common/Formatter.hpp"
#include "llvm/Support/raw_ostream.h"

namespace rgl {
ModuleSymbol::ModuleSymbol(const std::vector<std::string> &name,
                           std::shared_ptr<llvm::Module> module,
                           std::shared_ptr<llvm::IRBuilder<>> builder)
    : m_name(name), m_module(module), m_builder(builder), m_symbols({}) {}

ModuleSymbolPtr ModuleSymbol::make(const std::vector<std::string> &name,
                                   std::shared_ptr<llvm::LLVMContext> context) {
  std::string moduleName = Formatter<>::joinContainer('.', name);
  auto module = std::make_shared<llvm::Module>(std::move(moduleName), *context);
  auto builder = std::make_shared<llvm::IRBuilder<>>(*context);
  return ModuleSymbolPtr(new ModuleSymbol(name, module, builder));
}

std::string ModuleSymbol::toString() const {
  std::string str;
  llvm::raw_string_ostream rso{str};
  rso << *m_module;
  rso.flush();
  return str;
}
}; // namespace rgl
