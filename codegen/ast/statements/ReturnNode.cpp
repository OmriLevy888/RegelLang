#include "parser/ast/statements/ReturnNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
void ReturnNode::genCode() {
  auto retVal = m_expr->genCode();
  Context::getCurrContext()->builder()->CreateRet(retVal);
}
}; // namespace rgl
