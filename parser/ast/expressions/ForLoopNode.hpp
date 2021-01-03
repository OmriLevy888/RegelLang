#pragma once
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class ForLoopNode : public ExpressionNode {
public:
  ForLoopNode(Expression init, Expression cond, Expression advance, Block body)
      : m_init(std::move(init)), m_cond(std::move(cond)),
        m_advance(std::move(advance)), m_body(std::move(body)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 12, ' ');
    std::string initStr =
        (nullptr == m_init) ? ("no-init") : (m_init->toTreeStr(spaces + 17));
    std::string condStr =
        (nullptr == m_cond) ? ("no-cond") : (m_cond->toTreeStr(spaces + 16));
    std::string advanceStr = (nullptr == m_advance)
                                 ? ("no-advance")
                                 : (m_advance->toTreeStr(spaces + 19));
    return Formatter(
        "ForLoopNode<init:{},\n{}cond:{},\n{}advance:{},\n{}body:{}>", initStr,
        spacesStr, condStr, spacesStr, advanceStr, spacesStr,
        m_body->toTreeStr(spaces + 16));
  }

private:
  Expression m_init;
  Expression m_cond;
  Expression m_advance;
  Block m_body;
};
}; // namespace rgl
