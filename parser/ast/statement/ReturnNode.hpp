#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/statement/StatementNode.hpp"

namespace rgl {
class ReturnNode : public StatementNode {
public:
  ReturnNode(Expression expr = nullptr) : m_expr(std::move(expr)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string inner = "";
    if (nullptr != m_expr) {
      inner = Formatter("expr:{}", m_expr->toTreeStr(spaces + 16));
    }
    return Formatter("ReturnNode<{}>", inner);
  }

private:
  Expression m_expr;
};
}; // namespace rgl
