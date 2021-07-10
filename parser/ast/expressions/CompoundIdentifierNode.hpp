#pragma once
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class CompoundIdentifierNode : public IdentifierNode {
public:
  CompoundIdentifierNode(std::vector<std::string> &&value)
      : m_value(std::move(value)) {}
  CompoundIdentifierNode(const std::vector<std::string> &value)
      : m_value(value) {}

  virtual std::vector<std::string> get() const override;
  virtual std::string getString() const override;

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  std::vector<std::string> m_value;
};

using CompoundIdentifier = std::unique_ptr<CompoundIdentifierNode>;
}; // namespace rgl
