#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class IdentifierNode;
using Identifier = std::unique_ptr<IdentifierNode>;

class IdentifierNode : public ExpressionNode {
public:
  IdentifierNode(const std::vector<std::string> &value, const SourceRange &range = SourceRange{});
  IdentifierNode(std::vector<std::string> &&value, const SourceRange &range = SourceRange{});
  IdentifierNode(std::string &&value, const SourceRange &range = SourceRange{});

  size_t size() { return m_value.size(); }
  std::vector<std::string> &get() { return m_value; };
  std::string getString() const;

  bool equals(const Identifier &other) const;

  Identifier clone() const;

  virtual ValuePtr genCode() override;

  virtual std::string toTreeStr(size_t spaces) const override;

  virtual SourceRange getSourceRange() const override;

private:
  std::vector<std::string> m_value;
  SourceRange m_range;
};
}; // namespace rgl
