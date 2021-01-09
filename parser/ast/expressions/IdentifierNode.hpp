#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class IdentifierNode : public ExpressionNode {
public:
  IdentifierNode(std::string &&value) : m_value(value) {}

  inline const std::string &get() const noexcept { return m_value; }

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("Identifier<{}>", m_value);
  }

private:
  std::string m_value;
};

using Identifier = std::unique_ptr<IdentifierNode>;
}; // namespace rgl
