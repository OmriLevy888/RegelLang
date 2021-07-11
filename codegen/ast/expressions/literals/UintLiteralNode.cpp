#include "parser/ast/expressions/literals/UintLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"
#include "llvm/IR/Constants.h"

namespace rgl {
ValuePtr UintLiteralNode::genCode() {
  return std::make_shared<BasicValue>(llvm::ConstantInt::get(
      *Context::llvmContext(), llvm::APInt(m_type->getSizeBits(), m_value)));
}
}; // namespace rgl
