#pragma once
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class ConditionalNode : public ExpressionNode {
public:
  ConditionalNode(Expression cond, Block body, Expression _else)
      : m_cond(std::move(cond)), m_body(std::move(body)),
        m_else(std::move(_else)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    return Formatter("ConditionalNode<{}>", "hey");
  }

private:
  Expression m_cond;
  Block m_body;
  Expression m_else;
};
}; // namespace rgl
