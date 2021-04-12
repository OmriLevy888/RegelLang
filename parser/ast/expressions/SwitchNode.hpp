#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/constructs/Type.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/SwitchCaseNode.hpp"
#include <vector>

namespace rgl {
class SwitchNode : public ExpressionNode {
public:
  SwitchNode(Expression expr, std::vector<SwitchCase> &&cases)
      : m_expr(std::move(expr)), m_cases(std::move(cases)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 11, ' ');

    std::string casesStr = "no-cases";
    if (0 != m_cases.size()) {
      casesStr = "";
      auto it = m_cases.begin();
      for (; it + 1 != m_cases.cend(); it++) {
        casesStr +=
            Formatter("{},\n{}", (*it)->toTreeStr(spaces + 17), spacesStr);
      }
      casesStr += (*it)->toTreeStr(spaces + 17);
    }

    return Formatter("SwitchNode<expr:{},\n{}cases:{}>",
                     m_expr->toTreeStr(spaces + 16), spacesStr, casesStr);
  }

private:
  Expression m_expr;
  std::vector<SwitchCase> m_cases;
};
using Switch = std::unique_ptr<SwitchNode>;
}; // namespace rgl
