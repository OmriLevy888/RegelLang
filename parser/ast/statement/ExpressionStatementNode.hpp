#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/statement/StatementNode.hpp"

namespace rgl {
class ExpressionStatementNode : public StatementNode {
public:
  ExpressionStatementNode(Expression expr) : m_expr(std::move(expr)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    return Formatter("ExpressionStatement<expr:{}>",
                     m_expr->toTreeStr(spaces + 25));
  }

private:
  Expression m_expr;
};
}; // namespace rgl
