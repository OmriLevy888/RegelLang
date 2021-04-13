#include "parser/ast/expressions/literals/class-literal/ClassLiteralNode.hpp"
#include "common/Formatter.hpp"

namespace rgl {
std::string ClassLiteralNode::toTreeStr(size_t spaces) const {
  const std::string spacesStr(spaces + 17, ' ');
  std::string fieldsStr;
  if (0 == m_fields.size()) {
    fieldsStr = "no-params";
  } else {
    auto iter = m_fields.cbegin();
    for (; iter + 1 != m_fields.cend(); iter++) {
      fieldsStr += Formatter("{},\n{}", (*iter)->toTreeStr(spaces + 24),
                             std::string(spaces + 24, ' '));
    }
    fieldsStr += (*iter)->toTreeStr(spaces + 24);
  }

  std::string methodsStr;
  if (0 == m_methods.size()) {
    methodsStr = "no-methods";
  } else {
    auto iter = m_methods.cbegin();
    for (; iter + 1 != m_methods.cend(); iter++) {
      methodsStr += Formatter("{},\n{}", (*iter)->toTreeStr(spaces + 25),
                              std::string(spaces + 25, ' '));
    }
    methodsStr += (*iter)->toTreeStr(spaces + 25);
  }

  return Formatter("ClassLiteralNode<name:{},\n{}fields:{},\n{}methods:{}>",
                   m_name->toString(), spacesStr, fieldsStr, spaces,
                   methodsStr);
}
}; // namespace rgl
