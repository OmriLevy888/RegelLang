#pragma once

#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
namespace rgl {
class IndexNode : public ExpressionNode {
public:
  IndexNode(Expression expr, Expression index)
      : m_expr(std::move(expr)), m_index(std::move(index)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 6, ' ');
    return Formatter("Index<expr:{},\n{}index:{}>",
                     m_expr->toTreeStr(spaces + 11), spacesStr,
                     m_index->toTreeStr(spaces + 12));
  }

private:
  Expression m_expr;
  Expression m_index;
};
}; // namespace rgl
