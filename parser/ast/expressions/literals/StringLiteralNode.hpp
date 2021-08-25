#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class StringLiteralNode : public ExpressionNode {
public:
  StringLiteralNode(std::string &&value, const SourceRange &range = SourceRange{}) 
    : m_value(std::move(value)), m_range(range) {}

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("StringLiteral<{}>", m_value);
  }

  virtual SourceRange getSourceRange() const override { return m_range; }

private:
  std::string m_value;
  SourceRange m_range;
};
}; // namespace rgl
