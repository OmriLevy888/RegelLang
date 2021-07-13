#include "codegen/values/symbols/SymbolMap.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"
#include "codegen/values/symbols/FunctionSymbol.hpp"
#include "codegen/values/symbols/VariableSymbol.hpp"

namespace rgl {
SymbolMap::SymbolMap() : m_maps({}), m_symbol(nullptr) {}

SymbolPtr SymbolMap::get(const std::vector<std::string> &name) {
  return get(name.cbegin(), name.cend());
}
SymbolPtr SymbolMap::get(std::vector<std::string>::const_iterator curr,
                         std::vector<std::string>::const_iterator end) {
  if (curr == end) {
    return m_symbol;
  }

  auto nextMap = m_maps[*curr];
  if (nullptr == nextMap) {
    return nullptr;
  }
  return nextMap->get(curr + 1, end);
}

SymbolPtr SymbolMap::insert(const std::vector<std::string> &name,
                            SymbolPtr symbol) {
  return insert(name.cbegin(), name.cend(), symbol);
}
SymbolPtr SymbolMap::insert(std::vector<std::string>::const_iterator curr,
                            std::vector<std::string>::const_iterator end,
                            SymbolPtr symbol) {
  if (curr == end) {
    m_symbol = symbol;
    return symbol;
  }

  auto nextMap = m_maps[*curr];
  if (nullptr == nextMap) {
    nextMap = std::make_shared<SymbolMap>();
    m_maps[*curr] = nextMap;
  }
  return nextMap->insert(curr + 1, end, symbol);
}

FunctionSymbolPtr SymbolMap::createFunction(
    const std::vector<std::string> &name, TypeSymbolPtr retType,
    const std::vector<Parameter> &parameters, bool isVarArg) {
  auto functionSymbol =
      FunctionSymbol::make(name, retType, parameters, isVarArg);

  // Insert the function before generating the body so that recursive calls will
  // resolve correctly
  this->insert(name, functionSymbol);

  return functionSymbol;
}

// TODO: Implement these
TypeSymbolPtr SymbolMap::getType(const TypeNodePtr &typeNode) {
  return nullptr;
}
FunctionTypeSymbolPtr
SymbolMap::getFunctionType(std::vector<TypeSymbolPtr> &&params,
                           TypeSymbolPtr retType) {
  return nullptr;
}

VariableSymbolPtr
SymbolMap::createVariable(const std::vector<std::string> &name,
                          const Expression &value, TypeSymbolPtr type) {
  // TODO: change nullptr == value to value->isUndefined()
  TypeSymbolPtr variableType = nullptr;
  if (type->isConst() && nullptr == value) {
    // TODO: write error message
    return nullptr;
  }

  if (type->isImplicitType()) {
    if (nullptr == value) {
      // TODO: write error message
      return nullptr;
    }
    // Use implicit type from RHS
    variableType = value->getType();
  } else {
    // Use explicit type specified in declaration
    variableType = type;
  }

  auto variableSymbol = VariableSymbol::make(name, variableType);
  value->genCode(std::make_shared<BasicValue>(variableSymbol->llvmValue()));

  this->insert(name, variableSymbol);

  return variableSymbol;
}

VariableSymbolPtr
SymbolMap::createParameter(const std::vector<std::string> &name,
                           TypeSymbolPtr type, llvm::Value *paramValue) {
  auto parameterSymbol = VariableSymbol::make(name, type, paramValue);

  this->insert(name, parameterSymbol);

  return parameterSymbol;
}

void SymbolMap::clean() {
  for (auto &map : m_maps) {
    // Depth first
    if (nullptr == map.second) {
      // when lookin into a map to see if it contains an entry, the default
      // value is put there (nullptr) so skip it if it is there (much easier
      // than cleaning the null pointers)
      continue;
    }
    map.second->clean();
  }

  if (nullptr != m_symbol) {
    m_symbol->clean();
  }

  // TODO: check if this causes some weird problems, might have caused cycled
  // pointers(?)
  /* m_symbol = nullptr; */
}

std::string SymbolMap::toString() const {
  std::string ret = (nullptr == m_symbol) ? ("") : (m_symbol->toString());
  for (auto symbolMap : m_maps) {
    ret += Formatter("{}: <{}>", symbolMap.first, symbolMap.second->toString());
  }
  return ret;
}
}; // namespace rgl
