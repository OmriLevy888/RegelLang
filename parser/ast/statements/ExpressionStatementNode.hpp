#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"

namespace rgl {
class ExpressionStatementNode : public StatementNode {
public:
  ExpressionStatementNode() : m_expr(nullptr) {}
  ExpressionStatementNode(Expression expr) : m_expr(std::move(expr)) {}

  virtual void genCode() override;

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string exprStr = (nullptr == m_expr)
                              ? ("empty-statement")
                              : (m_expr->toTreeStr(spaces + 25));
    return Formatter("ExpressionStatement<expr:{}>", exprStr);
  }

private:
  Expression m_expr;
};
}; // namespace rgl
