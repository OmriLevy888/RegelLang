#include "parser/ast/expressions/ops/InvokeNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
TypeSymbolPtr InvokeNode::getType() const {
  // TODO: implement this
  return nullptr; 
}
  
ValuePtr InvokeNode::genCode() {
  auto callee = m_callee->genCode();
  if (!callee->success()) {
    // TODO: write error message
    return ValueBase::BadValue();
  }

  auto calleeType = callee->llvmType();
  if (!calleeType->isPointerTy() ||
      !calleeType->getPointerElementType()->isFunctionTy()) {
    // TODO: write error message
    return ValueBase::BadValue();
  }

  llvm::FunctionCallee functionCallee{reinterpret_cast<llvm::FunctionType *>(
                                          calleeType->getPointerElementType()),
                                      callee->llvmValue()};
  std::vector<llvm::Value *> args{}; // TODO: implement arguments
  return std::make_shared<BasicValue>(
      Context::builder()->CreateCall(functionCallee, args));
}
}; // namespace rgl
