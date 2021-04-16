#include "parser/ast/expressions/BasicIdentifierNode.hpp"

namespace rgl {
std::string BasicIdentifierNode::toTreeStr(size_t spaces) const {
  return Formatter("BasicIdentifier<value:{}>", m_value);
}
}; // namespace rgl
