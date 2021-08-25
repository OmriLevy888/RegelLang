#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class DoubleLiteralNode : public ExpressionNode {
public:
  DoubleLiteralNode(double value, const SourceRange &range = SourceRange{}) 
    : m_value(value), m_range(range) {}

  virtual TypeSymbolPtr getType() const override;

  virtual ValuePtr genCode() override;

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("DoubleLiteralNode<{}>", m_value);
  }

  virtual SourceRange getSourceRange() const override { return m_range; }

private:
  double m_value;
  SourceRange m_range;
};
}; // namespace rgl
