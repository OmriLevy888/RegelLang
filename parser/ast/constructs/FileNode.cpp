#include "parser/ast/constructs/FileNode.hpp"

namespace rgl {
std::string FileNode::toTreeStr(size_t spaces) const {
  std::string spacesStr(spaces + 9, ' ');
  std::string namespaceStr = (nullptr == m_namespace)
                                 ? ("global-namespace")
                                 : (m_namespace->toTreeStr(spaces));

  return Formatter("FileNode<namespace:{},\n{}body:{}>", namespaceStr,
                   spacesStr, m_body->toTreeStr(spaces + 14));
}
}; // namespace rgl
