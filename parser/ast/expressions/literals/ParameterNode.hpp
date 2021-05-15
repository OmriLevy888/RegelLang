#pragma once
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/constructs/Type.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class ParameterNode : public ASTNode {
public:
  ParameterNode(TypePtr type, Identifier name)
      : m_type(type), m_name(std::move(name)) {}

  TypePtr getType() { return m_type; }
  std::vector<std::string> getName() { return m_name->get(); }
  llvm::Type *getLLVMType() { return m_type->toLLVMType(); }

  std::string toTreeStr(size_t spaces) const override {
    const std::string spacesStr(spaces + 19, ' ');
    return Formatter("ParameterNode<name:{},\n{}type:{}>",
                     m_name->toTreeStr(spaces + 19), spacesStr, m_type);
  }

private:
  TypePtr m_type;
  Identifier m_name;
};

using Parameter = std::unique_ptr<ParameterNode>;
}; // namespace rgl
