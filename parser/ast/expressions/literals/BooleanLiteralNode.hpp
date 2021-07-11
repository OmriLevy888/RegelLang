#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class BooleanLiteralNode : public ExpressionNode {
public:
  BooleanLiteralNode(bool value) : m_value(value) {}

  virtual ValuePtr genCode() override;

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("BooleanLiteral<{}>", m_value);
  }

private:
  bool m_value;
};
}; // namespace rgl
