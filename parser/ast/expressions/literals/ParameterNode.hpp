#pragma once
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/constructs/TypeNodeBase.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class ParameterNode : public ASTNode {
public:
  ParameterNode(TypeNodePtr type, Identifier name)
      : m_type(std::move(type)), m_name(std::move(name)) {}

  TypeNodePtr &getType() { return m_type; }
  std::vector<std::string> getName() { return m_name->get(); }
  std::string getNameString() { return m_name->getString(); }

  std::string toTreeStr(size_t spaces) const override {
    const std::string spacesStr(spaces + 19, ' ');
    return Formatter("ParameterNode<name:{},\n{}type:{}>",
                     m_name->toTreeStr(spaces + 19), spacesStr,
                     m_type->toString());
  }

private:
  TypeNodePtr m_type;
  Identifier m_name;
};

using Parameter = std::unique_ptr<ParameterNode>;
}; // namespace rgl
