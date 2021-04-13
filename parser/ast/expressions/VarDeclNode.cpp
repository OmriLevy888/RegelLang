#include "parser/ast/expressions/VarDeclNode.hpp"

namespace rgl {

std::string VarDeclNode::toTreeStr(size_t spaces) const {
  std::string spacesStr(spaces + 8, ' ');
  std::string typeStr =
      (nullptr == m_type) ? ("implicit") : (m_type->toString());
  std::string exprStr =
      (nullptr == m_expr) ? ("default") : (m_expr->toTreeStr(spaces + 13));
  return Formatter("VarDecl<name:{},\n{}type:{},\n{}expr:{}>",
                   m_name->toTreeStr(spaces + 13), spacesStr, typeStr,
                   spacesStr, exprStr);
}

FieldPtr VarDeclNode::toFieldPtr(bool isExposed) {
  return std::make_unique<ClassFieldNode>(isExposed, std::move(m_type),
                                          std::move(m_name));
}
}; // namespace rgl
