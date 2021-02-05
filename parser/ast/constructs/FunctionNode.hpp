#pragma once
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/constructs/ParameterNode.hpp"

namespace rgl {
class FunctionNode : public ASTNode {
public:
  FunctionNode(Identifier name, TypePtr retType,
               std::vector<Parameter> &&params);

  virtual std::string toTreeStr(const size_t spaces) const override;

private:
  Identifier m_name;
  TypePtr m_retType;
  std::vector<Parameter> m_params;
};
}; // namespace rgl
