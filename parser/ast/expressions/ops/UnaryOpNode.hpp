#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include <unordered_map>

namespace rgl {
enum class UnaryOpType : uint8_t {
  u_err, // meta value

  u_pre_plus_plus,
  u_pre_minus_miuns,

  u_pre_exclamation,

  u_post_plus_plus,
  u_post_minus_minus,
};

class UnaryOpNode : public ExpressionNode {
public:
  UnaryOpNode(UnaryOpType type, Expression expr)
      : m_type(type), m_expr(std::move(expr)) {}

  virtual void setSwap(Expression expr) override { m_expr = std::move(expr); }

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 8, ' ');
    return Formatter("UnaryOp<op:{},\n{}expr:{}>", getOpString(), spacesStr,
                     m_expr->toTreeStr(spaces + 13));
  }

private:
  UnaryOpType m_type;
  Expression m_expr;

  std::string getOpString() const {
    static std::unordered_map<UnaryOpType, std::string> unaryOpReprMap{
        {UnaryOpType::u_err, "u_err"},

        {UnaryOpType::u_pre_plus_plus, "u_pre_plus_plus"},
        {UnaryOpType::u_pre_minus_miuns, "u_pre_minus_minus"},

        {UnaryOpType::u_pre_exclamation, "u_pre_exclamation"},

        {UnaryOpType::u_post_plus_plus, "u_post_plus_plus"},
        {UnaryOpType::u_post_minus_minus, "u_post_minus_minus"},
    };

    return unaryOpReprMap[m_type];
  }
};
}; // namespace rgl
