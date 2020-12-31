#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/statement/StatementNode.hpp"

namespace rgl {
class YieldNode : public StatementNode {
public:
  YieldNode(Expression expr = nullptr) : m_expr(std::move(expr)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string inner = "";
    if (nullptr != m_expr) {
      inner = Formatter("expr:{}", m_expr->toTreeStr(spaces + 16));
    }
    return Formatter("YieldNode<{}>", inner);
  }

private:
  Expression m_expr;
};
}; // namespace rgl
