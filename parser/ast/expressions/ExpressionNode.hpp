#pragma once
#include "common/Core.hpp"
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/Type.hpp"

namespace rgl {
class ExpressionNode : public ASTNode {
public:
  // virtual Type getType() = 0;
  // TODO: add getValue function that returns an LLVMValue

  // Returning true means we hit a leaf or something that can't be swapped with
  virtual void propagateLeft(std::unique_ptr<ExpressionNode> expr) {}
  virtual void setSwap(std::unique_ptr<ExpressionNode> expr) {}
  virtual bool isLeaf() { return true; }

  virtual std::string toString() const override { return toTreeStr(0); }
  virtual std::string toTreeStr(size_t spaces) const = 0;
};

using Expression = std::unique_ptr<ExpressionNode>;
}; // namespace rgl
