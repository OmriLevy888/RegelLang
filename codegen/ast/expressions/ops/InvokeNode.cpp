#include "parser/ast/expressions/ops/InvokeNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"

namespace rgl {
TypeSymbolPtr InvokeNode::getType() const {
  auto calleeType = m_callee->getType();
  if (calleeType->success() && calleeType->isFunctionType()) {
    auto functionType = std::dynamic_pointer_cast<FunctionTypeSymbol>(calleeType);
    return functionType->retType();
  }
  
  // TODO: implement this code path or add error message
  return TypeSymbolBase::BadValue(); 
}
  
ValuePtr InvokeNode::genCode() {
  auto callee = m_callee->genCode();
  if (!callee->success()) {
    // TODO: write error message
    return ValueBase::BadValue();
  }

  // TODO: change this to use FunctionSymbol invoke

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
