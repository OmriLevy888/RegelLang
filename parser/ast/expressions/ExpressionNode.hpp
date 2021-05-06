#pragma once
#include "common/Core.hpp"
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/constructs/BasicType.hpp"
#include "llvm/IR/Value.h"

namespace rgl {
class ExpressionNode : public ASTNode {
public:
  virtual TypePtr getType() const { return BasicType::t_void(); }
  // TODO: add getValue function that returns an LLVMValue
  // TODO: add getStorageLocation function that return an LLVMValue
  
  virtual llvm::Value *genCode() { return nullptr; }

  // Returning true means we hit a leaf or something that can't be swapped with
  virtual void propagateLeft(std::unique_ptr<ExpressionNode> expr) {}
  virtual void setSwap(std::unique_ptr<ExpressionNode> expr) {}
  virtual bool isLeaf() { return true; }
};

using Expression = std::unique_ptr<ExpressionNode>;
}; // namespace rgl
