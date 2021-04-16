#include "parser/ast/expressions/CompoundIdentifierNode.hpp"

namespace rgl {
std::string CompoundIdentifierNode::toTreeStr(size_t spaces) const {
  return Formatter("CompoundIdentifier<value:{}>",
                   Formatter<>::joinContainer('.', m_value));
}
} // namespace rgl
