#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/ScopeNode.hpp"

namespace rgl {
class ConditionalNode : public ExpressionNode {
public:
  ConditionalNode(Expression cond, Scope body, Expression _else)
      : m_cond(std::move(cond)), m_body(std::move(body)),
        m_else(std::move(_else)) {}
  ConditionalNode(Expression cond, Statement body, Expression _else = nullptr)
      : m_cond(std::move(cond)), m_else(std::move(_else)) {
    std::vector<Statement> statements;
    statements.push_back(std::move(body));
    m_body = std::make_unique<ScopeNode>(std::move(statements));
  }
  ConditionalNode(Expression cond, Statement body, Statement _else)
      : m_cond(std::move(cond)) {
    std::vector<Statement> bodyStatements;
    bodyStatements.push_back(std::move(body));
    m_body = std::make_unique<ScopeNode>(std::move(bodyStatements));

    std::vector<Statement> elseStatements;
    elseStatements.push_back(std::move(_else));
    m_else = std::make_unique<ScopeNode>(std::move(elseStatements));
  }

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 16, ' ');
    std::string elseStr =
        (nullptr == m_else) ? ("no-else") : (m_else->toTreeStr(spaces + 21));
    return Formatter("ConditionalNode<cond:{},\n{}body:{},\n{}else:{}>",
                     m_cond->toTreeStr(spaces + 21), spacesStr,
                     m_body->toTreeStr(spaces + 21), spacesStr, elseStr);
  }

private:
  Expression m_cond;
  Scope m_body;
  Expression m_else;
};
}; // namespace rgl
