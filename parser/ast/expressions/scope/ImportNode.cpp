#include "parser/ast/expressions/scope/ImportNode.hpp"

namespace rgl {
std::string ImportNode::toTreeStr(size_t spaces) const {
  return Formatter("ImportNode<name:{}>", m_name->toTreeStr(spaces));
}
}; // namespace rgl
