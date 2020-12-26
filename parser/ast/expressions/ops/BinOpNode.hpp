#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include <unordered_map>

namespace rgl {
enum class BinOpType : uint8_t {
  b_err, // meta value for comparisons as a default value

  b_dot,

  b_asterisk,
  b_forward_slash,

  b_plus,
  b_minus,

  b_equal_equal,
  b_not_equal,
  b_lesser_than,
  b_greater_than,
  b_lesser_equal,
  b_greater_equal,

  b_ampersand,
  b_caret,
  b_pipe,
};

class BinOpNode : public ExpressionNode {
public:
  BinOpNode(BinOpType op, Expression lhs, Expression rhs)
      : m_op(op), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual void propagateLeft(Expression expr) override {
    if (m_lhs->isLeaf()) {
      expr->setSwap(std::move(m_lhs));
      m_lhs = std::move(expr);
    } else {
      m_lhs->propagateLeft(std::move(expr));
    }
  }
  virtual void setSwap(Expression expr) override { m_rhs = std::move(expr); }
  virtual bool isLeaf() override { return false; }

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 6, ' ');
    return Formatter("BinOp<op:{},\n{}lhs:{},\n{}rhs:{}>", getOpString(),
                     spacesStr, m_lhs->toTreeStr(spaces + 10), spacesStr,
                     m_rhs->toTreeStr(spaces + 10));
  }

private:
  BinOpType m_op;
  Expression m_lhs;
  Expression m_rhs;

  std::string getOpString() const {
    static std::unordered_map<BinOpType, std::string> binOpReprMap{
        {BinOpType::b_dot, "b_dot"},

        {BinOpType::b_asterisk, "b_asterisk"},
        {BinOpType::b_forward_slash, "b_forward_slash"},

        {BinOpType::b_plus, "b_plus"},
        {BinOpType::b_minus, "b_minus"},

        {BinOpType::b_equal_equal, "b_equal_equal"},
        {BinOpType::b_not_equal, "b_not_equal"},
        {BinOpType::b_lesser_than, "b_lesser_than"},
        {BinOpType::b_greater_than, "b_greater_than"},
        {BinOpType::b_lesser_equal, "b_lesser_equal"},
        {BinOpType::b_greater_equal, "b_greater_equal"},

        {BinOpType::b_ampersand, "b_ampersand"},
        {BinOpType::b_caret, "b_caret"},
        {BinOpType::b_pipe, "b_pipe"},
    };

    return binOpReprMap[m_op];
  }
};
}; // namespace rgl
