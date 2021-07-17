#include "parser/ast/statements/ReturnNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/symbols/types/FunctionTypeSymbol.hpp"

namespace rgl {
void ReturnNode::genCode() {
  auto currFunctionSymbol =
      Context::getCurrContext()->getCurrGeneratedFunction();
  if (currFunctionSymbol->type()->retType() != m_expr->getType()) {
    // TODO: write error message
    return;
  }
  auto retVal = m_expr->genCode();
  Context::builder()->CreateRet(retVal->llvmValue());
}
}; // namespace rgl
