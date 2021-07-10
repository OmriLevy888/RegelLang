#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/FunctionSymbol.hpp"
#include "common/Formatter.hpp"
#include "llvm/IR/DerivedTypes.h"

namespace rgl {
llvm::Value *FunctionLiteralNode::genCode() {
  const bool isVarArg = false;
  auto functionSymbol = Context::module()->symbols().createFunction(
      m_name->get(), m_retType, m_parameters, isVarArg);
  functionSymbol->genCode(m_body);

  return functionSymbol->llvmFunction();
}

void FunctionLiteralNode::declare() {
  const bool isVarArg = false;
  auto functionSymbol = Context::module()->symbols().createFunction(
      m_name->get(), m_retType, m_parameters, isVarArg);
}
void FunctionLiteralNode::define() {
  auto symbol = Context::module()->symbols().get(m_name->get());
  if (nullptr == symbol || !symbol->isFunction()) {
    // TODO: propagate error;
  }
  FunctionSymbolPtr functionSymbol =
      std::dynamic_pointer_cast<FunctionSymbol>(symbol);
  functionSymbol->genCode(m_body, functionSymbol);
}
}; // namespace rgl
