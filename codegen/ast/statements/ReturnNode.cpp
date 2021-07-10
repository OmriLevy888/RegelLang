#include "parser/ast/statements/ReturnNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
void ReturnNode::genCode() {
  auto retVal = m_expr->genCode();
  Context::builder()->CreateRet(retVal);

  std::cout << Context::module()->toString() << std::endl;
}
}; // namespace rgl
