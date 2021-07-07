#pragma once
#include "codegen/ast/DeclarableBaseNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"

#include <vector>

namespace rgl {
class FunctionLiteralNode : public ExpressionNode, public DeclarableBaseNode {
public:
  FunctionLiteralNode(Identifier name, std::vector<Parameter> &&parameters,
                      TypePtr retType, Expression body)
      : m_name(std::move(name)), m_parameters(std::move(parameters)),
        m_retType(retType), m_body(std::move(body)) {}

  virtual llvm::Value *genCode() override;

  virtual void declare() override;
  virtual void define() override;

  std::string toTreeStr(size_t spaces) const override {
    const std::string spacesStr(spaces + 20, ' ');
    // replace "Anonymous" with line number/module name (?)
    const std::string nameStr =
        (nullptr != m_name) ? (m_name->toTreeStr(spaces + 25)) : ("anonymous");
    const std::string retTypeStr = m_retType->toString();
    std::string paramsStr{};
    if (0 == m_parameters.size()) {
      paramsStr = "no-params";
    } else {
      auto iter = m_parameters.cbegin();
      for (; iter + 1 != m_parameters.cend(); iter++) {
        paramsStr += Formatter("{},\n{}", (*iter)->toTreeStr(spaces + 26),
                               std::string(spaces + 31, ' '));
      }
      paramsStr += (*iter)->toTreeStr(spaces + 26);
    }
    return Formatter("FunctionLitearlNode<name:{},\n{}retType:{},\n{}"
                     "parameters:{},\n{}body:{}>",
                     nameStr, spacesStr, retTypeStr, spacesStr, paramsStr,
                     spacesStr, m_body->toTreeStr(spaces + 25));
  }

private:
  Identifier m_name;
  std::vector<Parameter> m_parameters;
  TypePtr m_retType;
  Expression m_body;
};

using FunctionPtr = std::unique_ptr<FunctionLiteralNode>;
}; // namespace rgl
