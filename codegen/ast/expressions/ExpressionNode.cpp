#include "parser/ast/expressions/ExpressionNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
llvm::Value *ExpressionNode::genCode() { return nullptr; }
llvm::Value *ExpressionNode::genCode(llvm::Value *storeLoc) {
  auto value = this->genCode();

  if (nullptr == storeLoc) {
    return value;
  } else if (nullptr == value) {
    // Propagate error
    return nullptr;
  }

  const bool isVolatile = false;
  return Context::builder()->CreateStore(value, storeLoc, isVolatile);
}
}; // namespace rgl
