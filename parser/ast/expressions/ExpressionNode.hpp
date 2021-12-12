#pragma once
#include "common/Core.hpp"
#include "parser/ast/ASTNode.hpp"

namespace rgl {
class ValueBase;
using ValuePtr = std::shared_ptr<ValueBase>;

class TypeSymbolBase;
using TypeSymbolPtr = std::shared_ptr<TypeSymbolBase>;

class ExpressionNode : public ASTNode {
public:
  virtual TypeSymbolPtr getType() const { return nullptr; }
  // TODO: add getStorageLocation function that return an LLVMValue

  virtual ValuePtr genCode();
  virtual ValuePtr genCode(ValuePtr storeLoc);

  // Returning true means we hit a leaf or something that can't be swapped with
  virtual void propagateLeft(std::unique_ptr<ExpressionNode> expr) {}
  virtual void setSwap(std::unique_ptr<ExpressionNode> expr) {}
  virtual bool isLeaf() { return true; }
};

using Expression = std::unique_ptr<ExpressionNode>;
}; // namespace rgl
