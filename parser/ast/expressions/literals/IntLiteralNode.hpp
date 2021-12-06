#pragma once
#include "common/Formatter.hpp"
#include "common/source-objects/SourceLocation.hpp"
#include "parser/ast/constructs/TypeNodeBase.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class IntLiteralNode : public ExpressionNode {
public:
  IntLiteralNode(int64_t value, TypeNodePtr type,
                 const SourceRange &range = SourceRange{})
      : m_value(value), m_type(std::move(type)), m_range(range) {}

  virtual TypeSymbolPtr getType() const override;

  virtual ValuePtr genCode() override;

  virtual std::string toTreeStr(size_t spaces) const noexcept override {
    return Formatter("IntLiteral<{}, {}>", m_value, m_type->toString());
  }

  virtual SourceRange getSourceRange() const override { return m_range; }

private:
  int64_t m_value;
  TypeNodePtr m_type;
  SourceRange m_range;
};
}; // namespace rgl
