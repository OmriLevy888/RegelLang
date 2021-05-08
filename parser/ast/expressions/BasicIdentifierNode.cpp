#include "parser/ast/expressions/BasicIdentifierNode.hpp"

namespace rgl {
std::vector<std::string> BasicIdentifierNode::get() const {
  return std::vector<std::string>{m_value};
}

std::string BasicIdentifierNode::toTreeStr(size_t spaces) const {
  return Formatter("BasicIdentifier<value:{}>", m_value);
}
}; // namespace rgl
