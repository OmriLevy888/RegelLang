#include "parser/ast/expressions/ScopeNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/SymbolMap.hpp"

namespace rgl {
template <typename TDeclarable>
void defineDeclareLoop(const std::vector<TDeclarable> &declarables) {
  for (auto &node : declarables) {
    node->declare();
  }

  for (auto &node : declarables) {
    node->define();
  }
}

llvm::Value *ScopeNode::genCode() {
  auto currFunction = Context::getCurrContext()->getCurrGeneratedFunction();

  if (nullptr != currFunction) {
    // when generating code in some function
    currFunction->createStackFrame();
  }

  /* defineDeclareLoop(m_classes); */
  if (0 != m_functions.size()) {
    defineDeclareLoop(m_functions);
  }

  for (auto &statement : m_statements) {
    statement->genCode();
  }

  if (nullptr != currFunction) {
    currFunction->removeStackFrame();
  }

  // TODO: fix this
  return nullptr;
}
}; // namespace rgl
