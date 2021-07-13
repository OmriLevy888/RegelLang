#include "codegen/values/symbols/VariableSymbol.hpp"
#include "codegen/Context.hpp"

namespace rgl {
VariableSymbol::VariableSymbol(llvm::Value *storeLoc, const std::string &name,
                               TypeSymbolPtr type, bool isParameter)
    : SymbolBase(storeLoc), m_name(name), m_type(type),
      m_isParameter(isParameter) {}

VariableSymbolPtr VariableSymbol::make(const std::vector<std::string> &name,
                                       TypeSymbolPtr type,
                                       llvm::Value *paramValue) {
  llvm::Value *arraySize = nullptr; // TODO: fix this
  const std::string joinedName = Formatter<>::joinContainer('.', name);

  bool isParameter = false;
  llvm::Value *storeLoc = paramValue;
  if (nullptr != storeLoc) {
    isParameter = true;
  } else {
    storeLoc = Context::builder()->CreateAlloca(type->llvmType(), arraySize,
                                                joinedName);
  }

  return VariableSymbolPtr(
      new VariableSymbol(storeLoc, joinedName, type, isParameter));
}

std::string VariableSymbol::toString() const { return "VariableSymbol"; }
}; // namespace rgl
