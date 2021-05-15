#include "codegen/VariableSymbol.hpp"
#include "codegen/Context.hpp"

namespace rgl {
VariableSymbol::VariableSymbol(TypePtr type, llvm::Value *storeLoc)
    : m_type(type), m_storeLoc(storeLoc) {}

VariableSymbolPtr VariableSymbol::make(const std::vector<std::string> &name,
                                       TypePtr type) {
  llvm::Value *arraySize = nullptr; // TODO: fix this
  auto storeLoc = Context::builder()->CreateAlloca(
      type->toLLVMType(), arraySize, Formatter<>::joinContainer('.', name));

  return VariableSymbolPtr(new VariableSymbol(type, storeLoc));
}

std::string VariableSymbol::toString() const { return "VariableSymbol"; }
}; // namespace rgl
