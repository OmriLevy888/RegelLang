#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/literals/class-literal/ClassFieldNode.hpp"

namespace rgl {
class VarDeclNode : public ExpressionNode {
public:
  VarDeclNode(Identifier name, TypePtr type, Expression expr)
      : m_name(std::move(name)), m_type(type), m_expr(std::move(expr)) {}

  virtual llvm::Value *genCode() override;

  FieldPtr toFieldPtr(bool isExposed = false, bool isMutable = false);

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  Identifier m_name;
  TypePtr m_type;
  Expression m_expr;
};

using VarDeclPtr = std::unique_ptr<VarDeclNode>;
}; // namespace rgl
