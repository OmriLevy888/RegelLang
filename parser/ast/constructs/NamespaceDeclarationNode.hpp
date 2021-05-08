#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class NamespaceDeclarationNode : public ConstructNode {
public:
  NamespaceDeclarationNode(Identifier name) : m_name(std::move(name)) {}

  std::vector<std::string> getName() { return m_name->get(); }

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  Identifier m_name;
};

using NamespaceDeclaration = std::unique_ptr<NamespaceDeclarationNode>;
}; // namespace rgl
