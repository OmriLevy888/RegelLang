#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class ParenthesesNode : public ExpressionNode {
public:
  ParenthesesNode(Expression expr) : m_expr(std::move(expr)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    return Formatter("Parentheses<expr:{}>", m_expr->toTreeStr(spaces + 17));
  }

private:
  Expression m_expr;
};
}; // namespace rgl
