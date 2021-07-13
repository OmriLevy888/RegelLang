#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
IdentifierNode::IdentifierNode(const std::vector<std::string> &value)
    : m_value(value) {}
IdentifierNode::IdentifierNode(std::vector<std::string> &&value)
    : m_value(std::move(value)) {}

std::string IdentifierNode::getString() const {
  return Formatter<>::joinContainer('.', m_value);
}

bool IdentifierNode::equals(const Identifier &other) const {
  if (this->m_value.size() != other->m_value.size()) {
    return false;
  }

  auto thisIt = this->m_value.cbegin();
  auto otherIt = other->m_value.cbegin();
  for (; thisIt != this->m_value.cend() && otherIt != other->m_value.cend();
       thisIt++, otherIt++) {
    if (*thisIt != *otherIt) {
      return false;
    }
  }

  return true;
}

Identifier IdentifierNode::clone() const {
  return std::make_unique<IdentifierNode>(m_value);
}

std::string IdentifierNode::toTreeStr(size_t spaces) const {
  return Formatter("IdentifierNode<value:{}>",
                   Formatter<>::joinContainer('.', m_value));
}
}; // namespace rgl
