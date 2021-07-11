#include "parser/ast/expressions/ExpressionNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
ValuePtr ExpressionNode::genCode() { return BasicValue::BadValue(); }
ValuePtr ExpressionNode::genCode(ValuePtr storeLoc) {
  auto value = this->genCode();

  if (nullptr == storeLoc) {
    return value;
  } else if (!value) {
    // Propagate error
    return BasicValue::BadValue();
  }

  const bool isVolatile = false;
  return std::make_shared<BasicValue>(Context::builder()->CreateStore(
      value->llvmValue(), storeLoc->llvmValue(), isVolatile));
}
}; // namespace rgl
