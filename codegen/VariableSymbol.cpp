#include "codegen/VariableSymbol.hpp"
#include "codegen/Context.hpp"

namespace rgl {
VariableSymbol::VariableSymbol(const std::string &name, TypePtr type,
                               llvm::Value *storeLoc, bool isParameter)
    : m_name(name), m_type(type), m_storeLoc(storeLoc),
      m_isParameter(isParameter) {}

VariableSymbolPtr VariableSymbol::make(const std::vector<std::string> &name,
                                       TypePtr type, llvm::Value *paramValue) {
  llvm::Value *arraySize = nullptr; // TODO: fix this
  const std::string joinedName = Formatter<>::joinContainer('.', name);

  bool isParameter = false;
  llvm::Value *storeLoc = paramValue;
  if (nullptr != storeLoc) {
    isParameter = true;
  } else {
    storeLoc = Context::builder()->CreateAlloca(type->toLLVMType(), arraySize,
                                                joinedName);
  }

  return VariableSymbolPtr(
      new VariableSymbol(joinedName, type, storeLoc, isParameter));
}

std::string VariableSymbol::toString() const { return "VariableSymbol"; }
}; // namespace rgl
