#pragma once
#include "common/Formatter.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class CharLiteralNode : public ExpressionNode {
public:
  CharLiteralNode(char value) : m_value(value) {}

  virtual TypePtr getType() const override { return BasicType::t_char(); }

  virtual llvm::Value *genCode() override;

  std::string toTreeStr(size_t spaces) const override {
    return Formatter("CharLiteral<{}>", m_value);
  }

private:
  char m_value;
};
}; // namespace rgl
