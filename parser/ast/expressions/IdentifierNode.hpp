#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class IdentifierNode : public ExpressionNode {
public:
  IdentifierNode(std::string &&value) : m_value(value) {}

  inline const std::string &get() const noexcept { return m_value; }

  std::string toString() const override {
    return Formatter("IdentifierNode<{}>", m_value);
  }

private:
  std::string m_value;
};
}; // namespace rgl
