#pragma once
#include "common/errors/ErrorManager.hpp"
#include "common/errors/ErrorObject.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/literals/class-literal/ClassFieldNode.hpp"
#include "parser/ast/expressions/literals/class-literal/MethodNode.hpp"
#include <vector>

namespace rgl {
class ClassLiteralNode : public ExpressionNode {
public:
  ClassLiteralNode(Identifier name, std::vector<FieldPtr> &&fields,
                   std::vector<MethodPtr> &&methods)
      : m_name(std::move(name)), m_fields(std::move(fields)),
        m_methods(std::move(methods)) {}

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  Identifier m_name;
  std::vector<FieldPtr> m_fields;
  std::vector<MethodPtr> m_methods;
};

using ClassPtr = std::unique_ptr<ClassLiteralNode>;
}; // namespace rgl
