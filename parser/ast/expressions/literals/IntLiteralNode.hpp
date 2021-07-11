#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/constructs/Type.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class IntLiteralNode : public ExpressionNode {
public:
  IntLiteralNode(int64_t value, std::shared_ptr<Type> type)
      : m_value(value), m_type(type) {}

  virtual TypePtr getType() const override { return m_type; }

  virtual ValuePtr genCode() override;

  virtual std::string toTreeStr(size_t spaces) const noexcept override {
    return Formatter("IntLiteral<{}, {}>", m_value, m_type);
  }

private:
  int64_t m_value;
  TypePtr m_type;
};
}; // namespace rgl
