#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
ValuePtr BinOpNode::genCode() {
  auto lhs = m_lhs->genCode();
  auto rhs = m_rhs->genCode();
  if (!lhs or !rhs) {
    // TODO: write error message
    return BasicValue::BadValue();
  }

  return std::make_shared<BasicValue>(
      Context::builder()->CreateAdd(lhs->llvmValue(), rhs->llvmValue(), "add"));
}
}; // namespace rgl
