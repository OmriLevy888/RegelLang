#pragma once
#include "parser/ast/constructs/ConstructNode.hpp"
#include "parser/ast/constructs/NamespaceDeclarationNode.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"
#include "parser/ast/expressions/literals/class-literal/ClassLiteralNode.hpp"

namespace rgl {
class FileNode : public ConstructNode {
public:
  FileNode(NamespaceDeclaration namespaceDecl, std::vector<ClassPtr> &&classes,
           std::vector<FunctionPtr> &&functions,
           std::vector<Expression> &&topLevelExprs)
      : m_namespace(std::move(namespaceDecl)), m_classes(std::move(classes)),
        m_functions(std::move(functions)),
        m_topLevelExpressions(std::move(topLevelExprs)) {}

  void genCode();

  // TODO: implement this
  virtual std::string toTreeStr(size_t spaces) const override;

private:
  NamespaceDeclaration m_namespace;
  std::vector<ClassPtr> m_classes;
  std::vector<FunctionPtr> m_functions;
  std::vector<Expression> m_topLevelExpressions;
};

using File = std::unique_ptr<FileNode>;
}; // namespace rgl
