#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class FloatLiteralNode : public ExpressionNode {
public:
  FloatLiteralNode(float value) : m_value(value) {}

  virtual TypeSymbolPtr getType() const override;

  virtual ValuePtr genCode() override;

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("FloatLiteral<{}>", m_value);
  }

private:
  float m_value;
};
}; // namespace rgl
