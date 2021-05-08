#include "parser/ast/expressions/literals/CharLiteralNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
llvm::Value *CharLiteralNode::genCode() {
  const uint64_t sizeBits = 8; // BasicType::t_char()->getSizeBits();
  return llvm::ConstantInt::get(
      *Context::llvmContext(),
      llvm::APInt(sizeBits, static_cast<uint64_t>(m_value)));
}
}; // namespace rgl
