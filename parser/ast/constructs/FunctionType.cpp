#include "parser/ast/constructs/FunctionType.hpp"

namespace rgl {
size_t FunctionType::getHash() const {
  if (!m_cachedHash.has_value()) {

    size_t h = this->Type::getHash();
    for (const auto param : m_params) {
      h ^= (param->getHash() << 1);
    }
    m_cachedHash = h;
  }

  return m_cachedHash.value();
}

TypePtr FunctionType::getOwningType() const {
  return FunctionType::make(m_params, m_retType,
                            m_typeProperties | TypeProperties::_owning);
}
TypePtr FunctionType::getMutableType() const {
  return FunctionType::make(m_params, m_retType,
                            m_typeProperties | TypeProperties::_mutable);
}
TypePtr FunctionType::getValueType() const {
  return FunctionType::make(m_params, m_retType,
                            m_typeProperties | ~TypeProperties::_isPointer);
}
TypePtr FunctionType::getUniquePointerType() const {
  return FunctionType::make(m_params, m_retType,
                            m_typeProperties | TypeProperties::_isPointer);
}
TypePtr FunctionType::getSharedPointerType() const {
  return FunctionType::make(m_params, m_retType,
                            m_typeProperties | TypeProperties::_isPointer |
                                TypeProperties::_isShared);
}

std::string FunctionType::toTreeStr(size_t spaces) const {
  std::string typePrefix = (m_typeProperties & TypeProperties::_owning) ? (":")
                           : (m_typeProperties & TypeProperties::_mutable)
                               ? ("&")
                               : ("");

  if (m_typeProperties & TypeProperties::_isPointer) {
    typePrefix +=
        (m_typeProperties & TypeProperties::_isShared) ? ("{}") : ("<>");
  }

  return Formatter(
      "FunctionType<{}func ({}) => {}>", typePrefix,
      Formatter<>::joinContainer(
          ", ", m_params,
          [spaces](auto paramType) { return paramType->toTreeStr(spaces); }),
      m_retType->toTreeStr(spaces));
}
}; // namespace rgl
