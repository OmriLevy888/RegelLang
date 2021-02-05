#include "parser/ast/constructs/ParameterNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
ParameterNode::ParameterNode(Identifier name, TypePtr type)
    : m_name(std::move(name)), m_type(type) {}

std::string ParameterNode::toTreeStr(const size_t spaces) const {
  return Formatter("Parameter<name:{}, type:{}>", m_name->toString(),
                   m_type->toString());
}
}; // namespace rgl
