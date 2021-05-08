#include "parser/ast/expressions/literals/FloatLiteralNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
llvm::Value *FloatLiteralNode::genCode() {
  return llvm::ConstantFP::get(*Context::llvmContext(), llvm::APFloat(m_value));
}
}; // namespace rgl
