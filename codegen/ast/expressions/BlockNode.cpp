#include "parser/ast/expressions/BlockNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/SymbolMap.hpp"

namespace rgl {
llvm::Value *BlockNode::genCode() {
  auto currFunction = Context::getCurrContext()->getCurrGeneratedFunction();

  if (nullptr != currFunction) {
    // when generating code in some function
    currFunction->createStackFrame();
  }

  for (auto &statement : m_statements) {
    statement->genCode();
  }

  if (nullptr != currFunction) {
    // khen generating code in some function
    currFunction->removeStackFrame();
  }

  // TODO: fix this
  return nullptr;
}
}; // namespace rgl
