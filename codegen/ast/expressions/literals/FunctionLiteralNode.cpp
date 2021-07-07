#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/FunctionSymbol.hpp"
#include "common/Formatter.hpp"
#include "llvm/IR/DerivedTypes.h"

namespace rgl {
llvm::Value *FunctionLiteralNode::genCode() {
  const bool isVarArg = false;
  auto functionSymbol = Context::module()->symbols().createFunction(
      m_name->get(), m_body, m_retType, m_parameters, isVarArg);

  return functionSymbol->llvmFunction();
}

// TODO: implement these
void FunctionLiteralNode::declare() {}
void FunctionLiteralNode::define() {}
}; // namespace rgl
