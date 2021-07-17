#include "parser/ast/expressions/literals/FloatLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
TypeSymbolPtr FloatLiteralNode::getType() const {
  return Context::module()->symbols().getType("float");
}

ValuePtr FloatLiteralNode::genCode() {
  return std::make_shared<BasicValue>(
      llvm::ConstantFP::get(*Context::llvmContext(), llvm::APFloat(m_value)));
}
}; // namespace rgl
