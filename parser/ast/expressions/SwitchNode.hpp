#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/Type.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/SwitchCaseNode.hpp"
#include <vector>

namespace rgl {
class SwitchNode : public ExpressionNode {
public:
  SwitchNode(Expression expr, TypePtr caseExprType,
             std::vector<SwitchCase> &&cases)
      : m_expr(std::move(expr)), m_caseExprType(caseExprType),
        m_cases(std::move(cases)) {}

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 11, ' ');
    std::string caseExprTypeStr = (nullptr == m_caseExprType)
                                      ? ("no-common-type")
                                      : (m_caseExprType->toString());

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

    return Formatter("SwitchNode<expr:{},\n{}caseExprType:{}\n{}cases:{}>",
                     m_expr->toTreeStr(spaces + 16), spacesStr, caseExprTypeStr,
                     spacesStr, casesStr);
  }

private:
  Expression m_expr;
  TypePtr m_caseExprType;
  std::vector<SwitchCase> m_cases;
};
using Switch = std::unique_ptr<SwitchNode>;
}; // namespace rgl