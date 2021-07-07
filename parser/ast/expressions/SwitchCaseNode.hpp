#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/ScopeNode.hpp"

namespace rgl {
class SwitchCaseNode : ASTNode {
public:
  SwitchCaseNode(Expression expr, Scope body)
      : m_expr(std::move(expr)), m_body(std::move(body)) {}
  SwitchCaseNode(Expression expr, Statement body) : m_expr(std::move(expr)) {
    std::vector<Statement> statements;
    statements.push_back(std::move(body));
    m_body = std::make_unique<ScopeNode>(std::move(statements));
  }

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 15, ' ');
    return Formatter("SwitchCaseNode<expr:{},\n{}body:{}>",
                     m_expr->toTreeStr(spaces + 20), spacesStr,
                     m_body->toTreeStr(spaces + 20));
  }

private:
  Expression m_expr;
  Scope m_body;
};
using SwitchCase = std::unique_ptr<SwitchCaseNode>;
}; // namespace rgl
