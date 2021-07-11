#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"
#include "llvm/IR/Constants.h"

namespace rgl {
ValuePtr IntLiteralNode::genCode() {
  const bool isSigned = true;
  return std::make_shared<BasicValue>(llvm::ConstantInt::get(
      *Context::llvmContext(),
      llvm::APInt(m_type->getSizeBits(), static_cast<uint64_t>(m_value),
                  isSigned)));
}
}; // namespace rgl
