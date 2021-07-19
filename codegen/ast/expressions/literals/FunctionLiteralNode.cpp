#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/symbols/FunctionSymbol.hpp"
#include "common/Formatter.hpp"
#include "llvm/IR/DerivedTypes.h"

namespace rgl {
ValuePtr FunctionLiteralNode::genCode() {
  const bool isVarArg = false;
  auto retTypeSymbol = Context::module()->symbols().getType(m_retType);
  auto functionSymbol = Context::module()->symbols().createFunction(
      m_name->get(), retTypeSymbol, m_parameters, isVarArg);
  functionSymbol->genCode(m_body);

  return functionSymbol;
}

void FunctionLiteralNode::declare() {
  const bool isVarArg = false;
  auto retTypeSymbol = Context::module()->symbols().getType(m_retType);
  auto functionSymbol = Context::module()->symbols().createFunction(
      m_name->get(), retTypeSymbol, m_parameters, isVarArg);
}
ValuePtr FunctionLiteralNode::define() {
  auto symbol = Context::module()->symbols().get(m_name->get());
  if (nullptr == symbol || !symbol->isFunction()) {
    // TODO: propagate error;
    return ValueBase::BadValue();
  }
  FunctionSymbolPtr functionSymbol =
      std::dynamic_pointer_cast<FunctionSymbol>(symbol);
  functionSymbol->genCode(m_body, functionSymbol);
  return functionSymbol;
}
}; // namespace rgl
