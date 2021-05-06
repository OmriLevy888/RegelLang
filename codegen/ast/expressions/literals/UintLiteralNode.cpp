#include "parser/ast/expressions/literals/UintLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "llvm/IR/Constants.h"

namespace rgl {
llvm::Value *UintLiteralNode::genCode() {
  return llvm::ConstantInt::get(*Context::llvmContext(),
                                llvm::APInt(m_type->getSizeBits(), m_value));
}
}; // namespace rgl
