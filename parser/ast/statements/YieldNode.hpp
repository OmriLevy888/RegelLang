#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"

namespace rgl {
class YieldNode : public StatementNode {
public:
  YieldNode() : m_expr(nullptr) {}
  YieldNode(Expression expr) : m_expr(std::move(expr)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string inner = "";
    if (nullptr != m_expr) {
      inner = Formatter("expr:{}", m_expr->toTreeStr(spaces + 16));
    }
    return Formatter("Yield<{}>", inner);
  }

private:
  Expression m_expr;
};
}; // namespace rgl
