#pragma once
#include "parser/ast/constructs/ConstructNode.hpp"
#include "parser/ast/constructs/NamespaceDeclarationNode.hpp"
#include "parser/ast/expressions/BlockNode.hpp"

namespace rgl {
class FileNode : public ConstructNode {
public:
  FileNode(NamespaceDeclaration namespaceDecl, Block body)
      : m_namespace(std::move(namespaceDecl)), m_body(std::move(body)) {}

  void genCode();

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  NamespaceDeclaration m_namespace;
  Block m_body;
};

using File = std::unique_ptr<FileNode>;
}; // namespace rgl
