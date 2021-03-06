#pragma once
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class WhileLoopNode : public ExpressionNode {
public:
  WhileLoopNode(Expression cond, Block body)
      : m_cond(std::move(cond)), m_body(std::move(body)) {}
  WhileLoopNode(Expression cond, Statement body) : m_cond(std::move(cond)) {
    std::vector<Statement> statements;
    statements.push_back(std::move(body));
    m_body = std::make_unique<BlockNode>(std::move(statements));
  }

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 14, ' ');
    std::string condStr =
        (nullptr == m_cond) ? ("no-cond") : (m_cond->toTreeStr(spaces + 19));
    return Formatter("WhileLoopNode<cond:{},\n{}body:{}>", condStr, spacesStr,
                     m_body->toTreeStr(spaces + 19));
  }

private:
  Expression m_cond;
  Block m_body;
};
}; // namespace rgl
