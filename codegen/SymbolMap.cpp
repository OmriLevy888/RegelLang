#include "codegen/SymbolMap.hpp"
#include "codegen/Context.hpp"
#include "codegen/FunctionSymbol.hpp"
#include "codegen/VariableSymbol.hpp"

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
    const std::vector<std::string> &name, const Expression &body,
    TypePtr retType, const std::vector<Parameter> &parameters, bool isVarArg) {
  auto functionSymbol =
      FunctionSymbol::make(name, retType, parameters, isVarArg);

  // Insert the function before generating the body so that recursive calls will
  // resolve correctly
  this->insert(name, functionSymbol);

  auto llvmFunction = functionSymbol->llvmFunction();
  auto entry =
      llvm::BasicBlock::Create(*Context::llvmContext(), "entry", llvmFunction);
  Context::builder()->SetInsertPoint(entry);

  // Create stack frame
  Context::getCurrContext()->pushGeneratedFunction(functionSymbol);
  body->genCode();
  Context::getCurrContext()->popGeneratedFunction();

  return functionSymbol;
}

VariableSymbolPtr
SymbolMap::createVariable(const std::vector<std::string> &name,
                          const Expression &value, TypePtr type) {
  TypePtr variableType = nullptr;
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
  value->genCode(variableSymbol->getStoreLoc());

  return variableSymbol;
}

void SymbolMap::clean() {
  for (auto &map : m_maps) {
    // Depth first
    map.second->clean();
  }

  if (nullptr != m_symbol) {
    m_symbol->clean();
  }

  // TODO: check if this causes some weird problems, might have caused cycled
  // pointers(?)
  m_symbol = nullptr;
}

std::string SymbolMap::toString() const { return "SymbolMap"; }
}; // namespace rgl
