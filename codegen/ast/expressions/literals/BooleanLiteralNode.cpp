#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
TypeSymbolPtr BooleanLiteralNode::getType() const {
  return Context::module()->symbols().getType("bool");
}

ValuePtr BooleanLiteralNode::genCode() {
  const uint64_t sizeBits = 1; // BasicType::t_bool()->getSizeBits();
  const uint64_t value = (m_value) ? (1ull) : (0ull);
  return std::make_shared<BasicValue>(llvm::ConstantInt::get(
      *Context::llvmContext(), llvm::APInt(sizeBits, value)));
}
}; // namespace rgl
