#include "parser/ast/expressions/VarDeclNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"
#include "common/Formatter.hpp"

namespace rgl {
ValuePtr VarDeclNode::genCode(ValuePtr storeLoc) {
  // create a variable on the current stack frame
  auto currStackFrame = Context::getCurrContext()
                            ->getCurrGeneratedFunction()
                            ->getCurrStackFrame();
  auto variableTypeSymbol = Context::module()->symbols().getType(m_type);
  currStackFrame->createVariable(m_name->get(), m_expr, variableTypeSymbol);

  // TODO: return the actual value
  return BasicValue::BadValue();
}
}; // namespace rgl
