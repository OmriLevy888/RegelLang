#include "parser/ast/expressions/BlockNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/SymbolMap.hpp"

namespace rgl {
llvm::Value *BlockNode::genCode() {
  auto currFunction = Context::getCurrContext()->getCurrGeneratedFunction();

  currFunction->createStackFrame();
  for (auto &statement : m_statements) {
    statement->genCode();
  }
  currFunction->removeStackFrame();

  // TODO: fix this
  return nullptr;
}
}; // namespace rgl
