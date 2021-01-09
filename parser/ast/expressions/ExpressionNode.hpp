#pragma once
#include "common/Core.hpp"
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/Type.hpp"

namespace rgl {
class ExpressionNode : public ASTNode {
public:
  virtual TypePtr getType() const { return Type::t_void(); }
  // TODO: add getValue function that returns an LLVMValue
  // TODO: add getStorageLocation function that return an LLVMValue

  // Returning true means we hit a leaf or something that can't be swapped with
  virtual void propagateLeft(std::unique_ptr<ExpressionNode> expr) {}
  virtual void setSwap(std::unique_ptr<ExpressionNode> expr) {}
  virtual bool isLeaf() { return true; }
};

using Expression = std::unique_ptr<ExpressionNode>;
}; // namespace rgl
