#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class CharLiteralNode : public ExpressionNode {
public:
  CharLiteralNode(char value) : m_value(value) {}

  virtual TypeSymbolPtr getType() const override;

  virtual ValuePtr genCode() override;

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("CharLiteral<{}>", m_value);
  }

private:
  char m_value;
};
}; // namespace rgl
