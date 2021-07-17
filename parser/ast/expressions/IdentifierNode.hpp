#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class IdentifierNode;
using Identifier = std::unique_ptr<IdentifierNode>;

class IdentifierNode : public ExpressionNode {
public:
  IdentifierNode(const std::vector<std::string> &value);
  IdentifierNode(std::vector<std::string> &&value);
  IdentifierNode(std::string &&value);

  size_t size() { return m_value.size(); }
  std::vector<std::string> &get() { return m_value; };
  std::string getString() const;

  bool equals(const Identifier &other) const;

  Identifier clone() const;

  virtual ValuePtr genCode() override;

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  std::vector<std::string> m_value;
};
}; // namespace rgl
