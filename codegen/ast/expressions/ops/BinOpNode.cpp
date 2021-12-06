#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
TypeSymbolPtr BinOpNode::getType() const {
  return Context::module()->symbols().getType("i32");
}

ValuePtr BinOpNode::genCode() {
  auto lhs = m_lhs->genCode();
  auto rhs = m_rhs->genCode();
  if (!lhs || !rhs) {
    // TODO: write error message
    return BasicValue::BadValue();
  }

  auto lhsType = m_lhs->getType();
  auto rhsType = m_rhs->getType();
  if (!lhsType || !rhsType) {
    // TODO: write error message
    return BasicValue::BadValue();
  }

  auto operator = Context::module()->symbols().getOperator(m_op);
  if (nullptr == operator) {
    // TODO: write error message
    return BasicValue::BadValue();
  }
  
  auto overload = operator->forTypes(lhsType, rhsType);
  if (nullptr == overload) {
    // TODO: write error message
    return BasicValue::BadValue();   
  }

  return overload->invoke(lhs, rhs);
  /* return std::make_shared<BasicValue>( */
  /*     Context::builder()->CreateAdd(lhs->llvmValue(), rhs->llvmValue(), "add")); */
}
}; // namespace rgl
