#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/ScopeNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"

namespace rgl {
class ForInLoopNode : public ExpressionNode {
public:
  ForInLoopNode(Identifier identifier, Expression iterable, Scope body)
      : m_identifier(std::move(identifier)), m_iterable(std::move(iterable)),
        m_body(std::move(body)) {}
  ForInLoopNode(Identifier identifier, Expression iterable, Statement body)
      : m_identifier(std::move(identifier)), m_iterable(std::move(iterable)) {
    std::vector<Statement> statements;
    statements.push_back(std::move(body));
    m_body = std::make_unique<ScopeNode>(std::move(statements));
  }

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 14, ' ');
    return Formatter("ForInLoopNode<identifier:{}\n{}iterable:{},\n{}body:{}>",
                     m_identifier->toTreeStr(spaces + 25), spacesStr,
                     m_iterable->toTreeStr(spaces + 24), spacesStr,
                     m_body->toTreeStr(spaces + 19));
  }

private:
  Identifier m_identifier;
  Expression m_iterable;
  Scope m_body;
};
}; // namespace rgl
