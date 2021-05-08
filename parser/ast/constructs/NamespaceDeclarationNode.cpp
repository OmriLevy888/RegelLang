#include "parser/ast/constructs/NamespaceDeclarationNode.hpp"

namespace rgl {
std::string NamespaceDeclarationNode::toTreeStr(size_t spaces) const {
  return Formatter("NamespaceDeclaration<{}>", m_name->toTreeStr(spaces));
}
}; // namespace rgl
