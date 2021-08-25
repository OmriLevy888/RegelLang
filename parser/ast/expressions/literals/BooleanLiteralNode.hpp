#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class BooleanLiteralNode : public ExpressionNode {
public:
  BooleanLiteralNode(bool value, const SourceRange &range = SourceRange{})
    : m_value(value), m_range(range) {}

  virtual TypeSymbolPtr getType() const override;

  virtual ValuePtr genCode() override;

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("BooleanLiteral<{}>", m_value);
  }

  virtual SourceRange getSourceRange() const override { return m_range; }

private:
  bool m_value;
  SourceRange m_range;
};
}; // namespace rgl
