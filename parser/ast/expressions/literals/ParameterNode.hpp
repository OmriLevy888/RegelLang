#pragma once
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/constructs/Type.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class ParameterNode : public ASTNode {
public:
  ParameterNode(TypePtr type, Identifier name)
      : m_type(type), m_name(std::move(name)) {}

  std::string toTreeStr(size_t spaces) const override {
    const std::string spacesStr(spaces + 18, ' ');
    return Formatter("ParameterNode<name:{},\n{}type:{}>",
                     m_name->toTreeStr(spaces + 18), spacesStr, m_type);
  }

private:
  TypePtr m_type;
  Identifier m_name;
};

using Parameter = std::unique_ptr<ParameterNode>;
}; // namespace rgl
