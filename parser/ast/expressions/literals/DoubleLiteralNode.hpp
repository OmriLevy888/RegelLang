#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class DoubleLiteralNode : public ExpressionNode {
public:
  DoubleLiteralNode(double value) : m_value(value) {}

  virtual TypeSymbolPtr getType() const override;

  virtual ValuePtr genCode() override;

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("DoubleLiteralNode<{}>", m_value);
  }

private:
  double m_value;
};
}; // namespace rgl
