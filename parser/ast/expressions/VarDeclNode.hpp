#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class VarDeclNode : public ExpressionNode {
public:
  VarDeclNode(Identifier name, TypePtr type, Expression expr)
      : m_name(std::move(name)), m_type(type), m_expr(std::move(expr)) {}

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  Identifier m_name;
  TypePtr m_type;
  Expression m_expr;
};
}; // namespace rgl
