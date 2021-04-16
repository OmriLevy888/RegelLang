#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class ClassFieldNode : public ExpressionNode {
public:
  ClassFieldNode(bool isExposed, TypePtr type, Identifier name)
      : m_isExposed(isExposed), m_type(type), m_name(std::move(name)) {}

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  bool m_isExposed;
  TypePtr m_type;
  Identifier m_name;
};

using FieldPtr = std::unique_ptr<ClassFieldNode>;
}; // namespace rgl
