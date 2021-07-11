#include "codegen/values/symbols/ModuleSymbol.hpp"
#include "common/Formatter.hpp"

namespace rgl {
ModuleSymbol::ModuleSymbol(const std::vector<std::string> &name,
                           llvm::Module *module,
                           std::shared_ptr<llvm::IRBuilder<>> builder)
    : SymbolBase(module), m_name(name), m_builder(builder), m_symbols({}) {}

ModuleSymbolPtr ModuleSymbol::make(const std::vector<std::string> &name,
                                   std::shared_ptr<llvm::LLVMContext> context) {
  std::string moduleName = Formatter<>::joinContainer('.', name);
  auto module = new llvm::Module(std::move(moduleName), *context);
  auto builder = std::make_shared<llvm::IRBuilder<>>(*context);
  return ModuleSymbolPtr(new ModuleSymbol(name, module, builder));
}
}; // namespace rgl
