#include "parser/ast/expressions/literals/CharLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
TypeSymbolPtr CharLiteralNode::getType() const {
  return Context::module()->symbols().getType("char");
}

ValuePtr CharLiteralNode::genCode() {
  const uint64_t sizeBits = 8; // BasicType::t_char()->getSizeBits();
  return std::make_shared<BasicValue>(llvm::ConstantInt::get(
      *Context::llvmContext(),
      llvm::APInt(sizeBits, static_cast<uint64_t>(m_value))));
}
}; // namespace rgl
