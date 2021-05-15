#include "parser/ast/expressions/VarDeclNode.hpp"
#include "codegen/Context.hpp"
#include "common/Formatter.hpp"

namespace rgl {
llvm::Value *VarDeclNode::genCode(llvm::Value *storeLoc) {
  // create a variable on the current stack frame
  auto currStackFrame = Context::getCurrContext()
                            ->getCurrGeneratedFunction()
                            ->getCurrStackFrame();
  currStackFrame->createVariable(m_name->get(), m_expr, m_type);

  // TODO: return the actual value
  return nullptr;
}
}; // namespace rgl
