#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class InvokeNode : public ExpressionNode {
public:
  InvokeNode(Expression callee, std::vector<Expression> &&params)
      : m_callee(std::move(callee)), m_params(std::move(params)) {}
  InvokeNode(Expression callee) : m_callee(std::move(callee)) {}
  InvokeNode(Expression callee, Expression param)
      : m_callee(std::move(callee)) {
    std::vector<Expression> params;
    params.push_back(std::move(param));
    m_params = std::move(params);
  }

  virtual llvm::Value *genCode() override;

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 7, ' ');
    std::string paramsStr;
    if (0 == m_params.size()) {
      paramsStr = "no-params";
    } else {
      std::string paramSpacesStr(spaces + 14, ' ');
      auto it = m_params.cbegin();
      for (; it + 1 != m_params.cend(); it++) {
        paramsStr +=
            Formatter("{},\n{}", (*it)->toTreeStr(spaces + 14), paramSpacesStr);
      }
      paramsStr += (*it)->toTreeStr(spaces + 14);
    }
    return Formatter("Invoke<callee:{}\n{}params:{}>",
                     m_callee->toTreeStr(spaces + 14), spacesStr, paramsStr);
  }

private:
  Expression m_callee;
  std::vector<Expression> m_params;
};
}; // namespace rgl
