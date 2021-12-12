#include "parser/ast/expressions/IdentifierNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"
#include "codegen/values/symbols/VariableSymbol.hpp"
#include "codegen/values/symbols/FunctionSymbol.hpp"

namespace rgl {
TypeSymbolPtr IdentifierNode::getType() const {
  if (m_cachedTypeSymbol) {
    return m_cachedTypeSymbol;
  }

  if (!lookupIdentifier()) {
    // TODO: write error message
    return TypeSymbolBase::BadValue();
  }
  return m_cachedTypeSymbol;
}

ValuePtr IdentifierNode::genCode() {
  std::string stringName = this->getString();
  if (!lookupIdentifier()) {
    // TODO: write error message
    return BasicValue::BadValue();
  }

  if (std::holds_alternative<VariableSymbolPtr>(m_cachedValueSymbol)) {
    auto variableSymbol = std::get<VariableSymbolPtr>(m_cachedValueSymbol);
    auto storeLoc = variableSymbol->llvmValue();
    if (variableSymbol->isParameter()) {
      // paramaters are not pointers, do not load them 
      return std::make_shared<BasicValue>(storeLoc);
    }
    return std::make_shared<BasicValue>(Context::builder()->CreateLoad(
          variableSymbol->llvmType(), storeLoc,
          Formatter("{}@load", variableSymbol->getName()).toString()));
  } else if (std::holds_alternative<FunctionSymbolPtr>(m_cachedValueSymbol)) {
    return std::get<FunctionSymbolPtr>(m_cachedValueSymbol);
  }

  // TODO: write error message
  return BasicValue::BadValue();
}

bool IdentifierNode::lookupIdentifier() const {
  std::vector<std::string> name = this->get();

  // search stack frames of current function
  auto stackFrames =
      Context::getCurrContext()->getCurrGeneratedFunction()->getStackFrames();
  for (auto frame = stackFrames.crbegin(); frame != stackFrames.crend();
       frame++) {
    // TODO: currently does not support functions defined inside other functions
    // WARN: fix this!
    auto variable = (*frame)->get(name);
    if (nullptr != variable) { // stack variable or parameter
      auto variableSymbol = std::dynamic_pointer_cast<VariableSymbol>(variable);
      m_cachedTypeSymbol = variableSymbol->getType();
      m_cachedValueSymbol = variableSymbol;
      return true;
    }
  }

  // look for globals
  auto global = Context::module()->symbols().get(name);
  if (nullptr != global) {
    if (global->isFunction()) {
      auto functionSymbol = std::dynamic_pointer_cast<FunctionSymbol>(global);
      m_cachedTypeSymbol = functionSymbol->type();
      m_cachedValueSymbol = functionSymbol;
      return true;
    } else if (global->isVariable()) { // global variable
      auto globalVarialbe = std::dynamic_pointer_cast<VariableSymbol>(global);
      m_cachedTypeSymbol = globalVarialbe->getType();
      m_cachedValueSymbol = globalVarialbe;
      return true;
    }

    // TODO: wrtie error message
    return false;
  }

  // TODO: look for types
  return false;
}
}; // namespace rgl
