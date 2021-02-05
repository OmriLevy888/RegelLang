#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class VarDeclNode : public ExpressionNode {
public:
  VarDeclNode(std::unique_ptr<IdentifierNode> name, TypePtr type,
              Expression expr)
      : m_name(std::move(name)), m_type(type), m_expr(std::move(expr)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 8, ' ');
    std::string typeStr =
        (nullptr == m_type) ? ("implicit") : (m_type->toString());
    std::string exprStr =
        (nullptr == m_expr) ? ("default") : (m_expr->toTreeStr(spaces + 13));
    return Formatter("VarDecl<name:{},\n{}type:{},\n{}expr:{}>",
                     m_name->toTreeStr(spaces + 13), spacesStr, typeStr,
                     spacesStr, exprStr);
  }

private:
  std::unique_ptr<IdentifierNode> m_name;
  TypePtr m_type;
  Expression m_expr;
};
}; // namespace rgl
