#include "parser/ast/expressions/literals/class-literal/MethodNode.hpp"
#include "common/Formatter.hpp"

namespace rgl {
std::string MethodNode::toTreeStr(size_t spaces) const {
  const std::string spacesStr(spaces + 11, ' ');
  const std::string isExposedStr = (m_isExposed) ? ("true") : ("false");
  return Formatter("MethodNode<isExposed:{},\n{}properties:{},\n{}func:{}>",
                   isExposedStr, spacesStr, std::to_string(m_properties),
                   spacesStr, m_func->toTreeStr(spaces + 16));
}
}; // namespace rgl

namespace std {
using namespace rgl;
std::string to_string(BitField<MethodProperties> properties) {
  std::vector<std::string> propertiesStr;
  if (properties & MethodProperties::_virtual) {
    propertiesStr.push_back("virtual");
  }

  if (0 == propertiesStr.size()) {
    return "UNKNOWN";
  }
  return Formatter<>::joinContainer(" ", propertiesStr);
}
}; // namespace std
