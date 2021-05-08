#include "parser/ast/expressions/BlockNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
llvm::Value *BlockNode::genCode() {
  for (auto &statement : m_statements) {
    statement->genCode();
  }

  // TODO: fix this
  return nullptr;
}
}; // namespace rgl
