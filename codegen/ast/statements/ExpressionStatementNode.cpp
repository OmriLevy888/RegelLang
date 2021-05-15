#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
void ExpressionStatementNode::genCode() {
  // Pass nullptr to invoke the codegen entry point
  m_expr->genCode(nullptr);
}
}; // namespace rgl
