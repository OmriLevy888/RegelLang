#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class BooleanLiteral : public ExpressionNode {
public:
  BooleanLiteral(bool value) : m_value(value) {}

  bool get() const noexcept { return m_value; }

  std::string toString() const override {
    return Formatter("BooleanLiteral<{}>", m_value);
  }

private:
  bool m_value;
};
}; // namespace rgl
