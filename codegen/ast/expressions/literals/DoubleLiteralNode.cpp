#include "parser/ast/expressions/literals/DoubleLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
ValuePtr DoubleLiteralNode::genCode() {
  return std::make_shared<BasicValue>(
      llvm::ConstantFP::get(*Context::llvmContext(), llvm::APFloat(m_value)));
}
}; // namespace rgl
