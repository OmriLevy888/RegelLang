#include "parser/ast/statements/ReturnNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/symbols/types/FunctionTypeSymbol.hpp"
#include "common/errors/ErrorManager.hpp"

namespace rgl {
ValuePtr ReturnNode::genCode() {
  auto currFunctionSymbol =
      Context::getCurrContext()->getCurrGeneratedFunction();
  auto funcRetType = currFunctionSymbol->type()->retType();
  auto exprType = m_expr->getType();
  if (funcRetType != exprType) {
    ErrorManager::logError(
        ErrorTypes::E_TYPE_MISMATCH,
        {Formatter("Function expects `{}` return type but got `{}`",
                   funcRetType->toString(), exprType->toString()),
         m_expr->getSourceRange()});
    return ValueBase::BadValue();
  }
  auto retVal = m_expr->genCode();
  Context::builder()->CreateRet(retVal->llvmValue());
  return ValueBase::Success();
}
}; // namespace rgl
