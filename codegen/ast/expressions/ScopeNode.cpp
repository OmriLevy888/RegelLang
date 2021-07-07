#include "parser/ast/expressions/ScopeNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/SymbolMap.hpp"
#include "common/utils/ScopedCallback.hpp"

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

  auto stackFrameCallback = UnarmedScopedCallback(
      [currFunction]() { currFunction->removeStackFrame(); });

  if (nullptr != currFunction) {
    // when generating code in some function
    currFunction->createStackFrame();
    stackFrameCallback.arm();
  }

  defineDeclareLoop(m_classes);
  defineDeclareLoop(m_functions);

  for (auto &statement : m_statements) {
    statement->genCode();
  }

  // TODO: fix this
  return nullptr;
}
}; // namespace rgl
