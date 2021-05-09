#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/FunctionSymbol.hpp"
#include "common/Formatter.hpp"
#include "llvm/IR/DerivedTypes.h"

namespace rgl {
llvm::Value *FunctionLiteralNode::genCode() {
  const bool isVarArg = false;
  auto functionSymbol =
      FunctionSymbol::make(m_name->get(), m_retType, m_parameters, isVarArg);
  Context::module()->symbols().insert(m_name->get(), functionSymbol);

  auto llvmFunction = functionSymbol->llvmFunction();
  auto entry =
      llvm::BasicBlock::Create(*Context::llvmContext(), "entry", llvmFunction);
  Context::builder()->SetInsertPoint(entry);

  m_body->genCode();

  return llvmFunction;
}
}; // namespace rgl
