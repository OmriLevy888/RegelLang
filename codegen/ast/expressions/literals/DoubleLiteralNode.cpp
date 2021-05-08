#include "parser/ast/expressions/literals/DoubleLiteralNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
llvm::Value *DoubleLiteralNode::genCode() {
  return llvm::ConstantFP::get(*Context::llvmContext(), llvm::APFloat(m_value));
}
}; // namespace rgl
