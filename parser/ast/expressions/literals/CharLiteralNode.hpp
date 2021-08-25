#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class CharLiteralNode : public ExpressionNode {
public:
  CharLiteralNode(char value, const SourceRange &range = SourceRange{})
    : m_value(value), m_range(range) {}

  virtual TypeSymbolPtr getType() const override;

  virtual ValuePtr genCode() override;

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("CharLiteral<{}>", m_value);
  }

  virtual SourceRange getSourceRange() const override { return m_range; }

private:
  char m_value;
  SourceRange m_range;
};
}; // namespace rgl
