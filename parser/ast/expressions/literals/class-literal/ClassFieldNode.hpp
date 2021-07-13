#pragma once
#include "parser/ast/constructs/TypeNodeBase.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class ClassFieldNode : public ExpressionNode {
public:
  ClassFieldNode(bool isExposed, TypeNodePtr type, Identifier name)
      : m_isExposed(isExposed), m_type(std::move(type)),
        m_name(std::move(name)) {}

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  bool m_isExposed;
  TypeNodePtr m_type;
  Identifier m_name;
};

using FieldPtr = std::unique_ptr<ClassFieldNode>;
}; // namespace rgl
