#include "parser/ast/constructs/FunctionType.hpp"

namespace rgl {
bool FunctionType::operator==(TypePtr other) const {
  // TODO: implement this
  return false;
}

size_t FunctionType::getHash() const {
  size_t h = this->Type::getHash();
  for (const auto param : m_params) {
    h ^= (param->getHash() << 1);
  }
  return h;
}

std::string FunctionType::toTreeStr(size_t spaces) const { return ""; }

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
};  // namespace rgl