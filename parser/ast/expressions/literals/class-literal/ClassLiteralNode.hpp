#pragma once
#include "codegen/ast/DeclarableNodeBase.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/literals/class-literal/ClassFieldNode.hpp"
#include "parser/ast/expressions/literals/class-literal/MethodNode.hpp"

#include <vector>

namespace rgl {
class ClassLiteralNode : public ExpressionNode, public DeclarableNodeBase {
public:
  ClassLiteralNode(Identifier name, std::vector<FieldPtr> &&fields,
                   std::vector<MethodPtr> &&methods)
      : m_name(std::move(name)), m_fields(std::move(fields)),
        m_methods(std::move(methods)) {}

  // TODO: implement these
  virtual void declare() override {}
  virtual void define() override {}

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  Identifier m_name;
  std::vector<FieldPtr> m_fields;
  std::vector<MethodPtr> m_methods;
};

using ClassPtr = std::unique_ptr<ClassLiteralNode>;
}; // namespace rgl
