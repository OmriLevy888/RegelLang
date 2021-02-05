#pragma once
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/Type.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class ParameterNode : public ASTNode {
public:
  ParameterNode(Identifier name, TypePtr type);

  virtual std::string toTreeStr(const size_t spaces) const override;

private:
  Identifier m_name;
  TypePtr m_type;
};

using Parameter = std::unique_ptr<ParameterNode>;
}; // namespace rgl
