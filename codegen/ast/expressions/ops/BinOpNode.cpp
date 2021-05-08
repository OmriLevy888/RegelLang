#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
llvm::Value *BinOpNode::genCode() {
  auto lhs = m_lhs->genCode();
  auto rhs = m_rhs->genCode();
  return Context::builder()->CreateAdd(lhs, rhs, "add");
}
}; // namespace rgl
