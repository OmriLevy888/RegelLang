#include "parser/ast/expressions/ops/InvokeNode.hpp"
#include "codegen/Context.hpp"

namespace rgl {
llvm::Value *InvokeNode::genCode() {
  std::cout << this->toString() << std::endl;
  auto callee = m_callee->genCode();
  if (nullptr == callee) {
    // TODO: write error message
    return nullptr;
  }

  auto calleeType = callee->getType();
  if (!calleeType->isPointerTy() ||
      !calleeType->getPointerElementType()->isFunctionTy()) {
    // TODO: write error message
    return nullptr;
  }

  llvm::FunctionCallee functionCallee{reinterpret_cast<llvm::FunctionType *>(
                                          calleeType->getPointerElementType()),
                                      callee};
  std::vector<llvm::Value *> args{}; // TODO: implement arguments
  return Context::builder()->CreateCall(functionCallee, args);
}
}; // namespace rgl
