#include "parser/ast/constructs/FunctionNode.hpp"
#include "common/Formatter.hpp"

namespace rgl {
FunctionNode::FunctionNode(Identifier name, TypePtr retType,
                           std::vector<Parameter> &&params)
    : m_name(std::move(name)), m_retType(retType), m_params(std::move(params)) {
}

std::string FunctionNode::toTreeStr(const size_t spaces) const {
  const std::string retTypeStr =
      (nullptr == m_retType) ? ("auto-deduce") : (m_retType->toString());

  std::string paramsStr = "no-params";
  if (0 != m_params.size()) {
    paramsStr = "";
    const std::string spacesStr(spaces + 20, ' ');
    auto it = m_params.cbegin();
    for (; it + 1 != m_params.cend(); it++) {
      paramsStr +=
          Formatter("{},\n{}", (*it)->toTreeStr(spaces + 20), spacesStr);
    }
    paramsStr += Formatter("{}", (*it)->toTreeStr(spaces + 20));
  }

  return Formatter("Function<name:{},\n{}retType:{},\n{}params:{}>",
                   m_name->toTreeStr(spaces), retTypeStr);
}
}; // namespace rgl
