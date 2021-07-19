#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
ValuePtr ExpressionStatementNode::genCode() {
  // Pass nullptr to invoke the codegen entry point
  return m_expr->genCode(nullptr);
}
}; // namespace rgl
