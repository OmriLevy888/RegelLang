#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class StringLiteralNode : public ExpressionNode {
public:
  StringLiteralNode(std::string &&value) : m_value(std::move(value)) {}

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("StringLiteral<{}>", m_value);
  }

private:
  std::string m_value;
};
}; // namespace rgl
