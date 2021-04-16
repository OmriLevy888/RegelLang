#pragma once
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class BasicIdentifierNode : public IdentifierNode {
public:
  BasicIdentifierNode(std::string &&value) : m_value(std::move(value)) {}
  BasicIdentifierNode(const std::string &value) : m_value(value) {}

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  std::string m_value;
};

using BasicIdentifier = std::unique_ptr<BasicIdentifierNode>;
}; // namespace rgl
