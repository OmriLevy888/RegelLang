#pragma once
#include "parser/ast/constructs/ConstructNode.hpp"
#include "parser/ast/constructs/NamespaceDeclarationNode.hpp"
#include "parser/ast/expressions/ScopeNode.hpp"

namespace rgl {
class FileNode : public ConstructNode {
public:
  FileNode(NamespaceDeclaration namespaceDecl, Scope body)
      : m_namespace(std::move(namespaceDecl)), m_body(std::move(body)) {}

  void genCode();

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  NamespaceDeclaration m_namespace;
  Scope m_body;
};

using File = std::unique_ptr<FileNode>;
}; // namespace rgl
