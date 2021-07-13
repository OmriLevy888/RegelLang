#include "parser/ast/constructs/FunctionTypeNode.hpp"

namespace rgl {
TypeNodePtr FunctionTypeNode::getOwningType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() | TypeProperties::_owning;
  return ret;
}
TypeNodePtr FunctionTypeNode::getMutableType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() | TypeProperties::_mutable;
  return ret;
}
TypeNodePtr FunctionTypeNode::getValueType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() & ~TypeProperties::_isPointer &
                          ~TypeProperties::_isShared;
  return ret;
}
TypeNodePtr FunctionTypeNode::getUniquePointerType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() | TypeProperties::_isPointer;
  return ret;
}
TypeNodePtr FunctionTypeNode::getSharedPointerType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() | TypeProperties::_isPointer |
                          TypeProperties::_isShared;
  return ret;
}

TypeNodePtr FunctionTypeNode::clone() const {
  std::vector<TypeNodePtr> clonedParams;
  clonedParams.reserve(m_params.size());
  for (const auto &paramType : m_params) {
    clonedParams.push_back(paramType->clone());
  }
  auto clonedRetType = m_retType->clone();

  return std::make_unique<FunctionTypeNode>(
      std::move(clonedParams), std::move(clonedRetType), m_typeProperties);
}

std::string FunctionTypeNode::toTreeStr(size_t spaces) const {
  std::string typePrefix = (m_typeProperties & TypeProperties::_owning) ? (":")
                           : (m_typeProperties & TypeProperties::_mutable)
                               ? ("&")
                               : ("");

  if (m_typeProperties & TypeProperties::_isPointer) {
    typePrefix +=
        (m_typeProperties & TypeProperties::_isShared) ? ("{}") : ("<>");
  }

  return Formatter("FunctionType<{}func ({}) => {}>", typePrefix,
                   Formatter<>::joinContainer(", ", m_params,
                                              [spaces](const auto &paramType) {
                                                return paramType->toTreeStr(
                                                    spaces);
                                              }),
                   m_retType->toTreeStr(spaces));
}
}; // namespace rgl
