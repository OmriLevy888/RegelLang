#include "codegen/Context.hpp"
#include "common/Formatter.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "llvm/IR/DerivedTypes.h"

namespace rgl {
llvm::Value *FunctionLiteralNode::genCode() {
  const bool isVarArg = false;
  auto functionTableEntry = FunctionTableEntry::make(m_name->get(), m_retType,
                                                     m_parameters, isVarArg);
  Context::module()->functions().insertFunction(m_name->get(),
                                                functionTableEntry);

  auto llvmFunction = functionTableEntry->llvmFunction();
  auto entry =
      llvm::BasicBlock::Create(*Context::llvmContext(), "entry", llvmFunction);
  Context::builder()->SetInsertPoint(entry);

  m_body->genCode();

  return llvmFunction;
}
}; // namespace rgl
