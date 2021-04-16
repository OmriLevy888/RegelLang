#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class NamespaceDeclarationNode : public ExpressionNode {
public:
  NamespaceDeclarationNode(Identifier name) : m_name(std::move(name)) {}

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  Identifier m_name;
};

using NamespaceDeclaration = std::unique_ptr<NamespaceDeclarationNode>;
}; // namespace rgl
