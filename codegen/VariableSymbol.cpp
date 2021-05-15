#include "codegen/VariableSymbol.hpp"
#include "codegen/Context.hpp"

namespace rgl {
VariableSymbol::VariableSymbol(const std::string &name, TypePtr type,
                               llvm::Value *storeLoc)
    : m_name(name), m_type(type), m_storeLoc(storeLoc) {}

VariableSymbolPtr VariableSymbol::make(const std::vector<std::string> &name,
                                       TypePtr type) {
  llvm::Value *arraySize = nullptr; // TODO: fix this
  std::string joinedName = Formatter<>::joinContainer('.', name);
  auto storeLoc = Context::builder()->CreateAlloca(type->toLLVMType(),
                                                   arraySize, joinedName);

  return VariableSymbolPtr(new VariableSymbol(joinedName, type, storeLoc));
}

std::string VariableSymbol::toString() const { return "VariableSymbol"; }
}; // namespace rgl
