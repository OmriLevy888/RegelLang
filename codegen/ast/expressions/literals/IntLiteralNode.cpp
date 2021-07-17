#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
TypeSymbolPtr IntLiteralNode::getType() const {
  return Context::module()->symbols().getType(m_type);
}

ValuePtr IntLiteralNode::genCode() {
  const bool isSigned = true;
  auto typeSymbol = Context::module()->symbols().getType(m_type);
  return std::make_shared<BasicValue>(llvm::ConstantInt::get(
      *Context::llvmContext(),
      llvm::APInt(typeSymbol->getSizeBits(), static_cast<uint64_t>(m_value),
                  isSigned)));
}
}; // namespace rgl
