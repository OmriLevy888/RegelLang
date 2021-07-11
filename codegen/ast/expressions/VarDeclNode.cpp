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
  currStackFrame->createVariable(m_name->get(), m_expr, m_type);

  // TODO: return the actual value
  return BasicValue::BadValue();
}
}; // namespace rgl
