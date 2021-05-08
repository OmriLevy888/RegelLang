#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
llvm::Value *BooleanLiteralNode::genCode() {
  const uint64_t sizeBits = 1; // BasicType::t_bool()->getSizeBits();
  const uint64_t value = (m_value) ? (1ull) : (0ull);
  return llvm::ConstantInt::get(*Context::llvmContext(),
                                llvm::APInt(sizeBits, value));
}
}; // namespace rgl
