#include "parser/ast/expressions/literals/UintLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"
#include "llvm/IR/Constants.h"

namespace rgl {
ValuePtr UintLiteralNode::genCode() {
  auto typeSymbol = Context::module()->symbols().getType(m_type);
  return std::make_shared<BasicValue>(
      llvm::ConstantInt::get(*Context::llvmContext(),
                             llvm::APInt(typeSymbol->getSizeBits(), m_value)));
}
}; // namespace rgl
