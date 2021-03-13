#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"

#include <vector>

namespace rgl {
class FunctionLietralNode : public ExpressionNode {
public:
  FunctionLietralNode(Identifier name, std::vector<ParameterNode> &&parameters,
                      TypePtr retType, Expression body)
      : m_name(std::move(name)), m_parameters(std::move(parameters)),
        m_retType(retType), m_body(std::move(body)) {}

  std::string toTreeStr(size_t spaces) const override {
    const std::string spacesStr(spaces + 20, ' ');
    // replace "Anonymous" with line number/module name (?)
    const std::string nameStr =
        (nullptr == m_name) ? (m_name->toTreeStr(spaces + 25)) : ("Anonymous");
    const std::string retTypeStr =
        (nullptr == m_retType) ? ("deduce") : (m_retType->toString());
    std::string paramsStr;
    if (0 == m_parameters.size()) {
      paramsStr = "no-params";
    } else {
      std::vector<std::string> formattedParams;
      formattedParams.reserve(m_parameters.size());
      std::transform(
          m_parameters.cbegin(), m_parameters.cend(), formattedParams.begin(),
          [spaces](auto node) { return node->toTreeStr(spaces + 27); });
      paramsStr = Formatter<>::joinIter(
          Formatter(",\n{}", std::string(spaces + 31, ' ')),
          formattedParams.cbegin(), formattedParams.cend());
    }
    return Formatter("FunctionLietarlNode<name:{},\n{}retType:{},\n{}"
                     "parameters:{},\n{}body:{}>",
                     nameStr, spacesStr, retTypeStr, spacesStr, paramsStr,
                     spacesStr, m_body->toTreeStr(spaces + 25));
  }

private:
  Identifier m_name;
  std::vector<ParameterNode> m_parameters;
  TypePtr m_retType;
  Expression m_body;
};
}; // namespace rgl
