#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class FloatLiteralNode : public ExpressionNode {
public:
  FloatLiteralNode(float value, const SourceRange &range = SourceRange{})
    : m_value(value), m_range(range) {}

  virtual TypeSymbolPtr getType() const override;

  virtual ValuePtr genCode() override;

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("FloatLiteral<{}>", m_value);
  }

  virtual SourceRange getSourceRange() const override { return m_range; }

private:
  float m_value;
  SourceRange m_range;
};
}; // namespace rgl
