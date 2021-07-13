#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/constructs/TypeNodeBase.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class UintLiteralNode : public ExpressionNode {
public:
  UintLiteralNode(int64_t value, TypeNodePtr type)
      : m_value(value), m_type(std::move(type)) {}

  virtual ValuePtr genCode() override;

  virtual std::string toTreeStr(size_t spaces) const noexcept override {
    return Formatter("UintLiteral<{}, {}>", m_value, m_type->toString());
  }

private:
  uint64_t m_value;
  TypeNodePtr m_type;
};
}; // namespace rgl
