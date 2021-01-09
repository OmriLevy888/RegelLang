#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/Type.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class UintLiteralNode : public ExpressionNode {
public:
  UintLiteralNode(int64_t value, std::shared_ptr<Type> type)
      : m_value(value), m_type(type) {}

  std::string toTreeStr(size_t spaces) const noexcept {
    return Formatter("UintLiteral<{}, {}>", m_value, *m_type);
  }

private:
  uint64_t m_value;
  std::shared_ptr<Type> m_type;
};
}; // namespace rgl
