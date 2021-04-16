#include "parser/ast/expressions/literals/class-literal/ClassFieldNode.hpp"
#include "common/Formatter.hpp"

namespace rgl {
std::string ClassFieldNode::toTreeStr(size_t spaces) const {
  const std::string spacesStr(spaces + 15, ' ');
  const std::string isExposedStr = (m_isExposed) ? ("true") : ("false");
  return Formatter("ClassFieldNode<isExposed:{}\n{}type:{}\n{}name:{}>",
                   isExposedStr, spacesStr, m_type->toTreeStr(spaces + 20),
                   spacesStr, m_name->toTreeStr(spaces + 20));
}
}; // namespace rgl
