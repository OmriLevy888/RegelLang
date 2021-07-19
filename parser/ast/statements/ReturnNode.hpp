#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"

namespace rgl {
class ReturnNode : public StatementNode {
public:
  ReturnNode() : m_expr(nullptr) {}
  ReturnNode(Expression expr) : m_expr(std::move(expr)) {}

  virtual ValuePtr genCode() override;

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string inner = "";
    if (nullptr != m_expr) {
      inner = Formatter("expr:{}", m_expr->toTreeStr(spaces + 12));
    }
    return Formatter("Return<{}>", inner);
  }

private:
  Expression m_expr;
};
}; // namespace rgl
