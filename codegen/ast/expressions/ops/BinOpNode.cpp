#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

#include <iostream>

namespace rgl {
TypeSymbolPtr BinOpNode::getType() const {
  return Context::module()->symbols().getType("i32");
}

ValuePtr BinOpNode::genCode() {
  auto lhs = m_lhs->genCode();
  auto rhs = m_rhs->genCode();
  if (!lhs->success() || !rhs->success()) {
    // TODO: write error message
    return BasicValue::BadValue();
  }

  auto lhsType = m_lhs->getType();
  auto rhsType = m_rhs->getType();
  if (!lhsType || !rhsType) {
    // TODO: write error message
    return BasicValue::BadValue();
  }

  auto binOp = Context::module()->symbols().getOperator(m_op);
  if (nullptr == binOp) {
    // TODO: write error message
    return BasicValue::BadValue();
  }

  std::cout << "Found operator" << std::endl;

  // TODO: uncomment this code
  /*   auto overload = operator->forTypes(lhsType, rhsType); */
  /*   if (nullptr == overload) { */
  /*     // TODO: write error message */
  /*     return BasicValue::BadValue();    */
  /*   } */
  /*  */
  /*   return overload->invoke(lhs, rhs); */

  return nullptr;
  /* return std::make_shared<BasicValue>( */
  /*     Context::builder()->CreateAdd(lhs->llvmValue(), rhs->llvmValue(),
   * "add")); */
}
}; // namespace rgl
