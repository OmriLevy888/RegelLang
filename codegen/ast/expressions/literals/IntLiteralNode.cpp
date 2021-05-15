#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "llvm/IR/Constants.h"

namespace rgl {
llvm::Value *IntLiteralNode::genCode() {
  const bool isSigned = true;
  return llvm::ConstantInt::get(*Context::llvmContext(),
                                llvm::APInt(m_type->getSizeBits(),
                                            static_cast<uint64_t>(m_value),
                                            isSigned));
}
}; // namespace rgl
