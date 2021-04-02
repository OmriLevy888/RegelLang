#include "parser/ast/constructs/BasicType.hpp"

namespace rgl {
bool BasicType::operator==(TypePtr other) const {
  // TODO: implement this
  return false;
}

bool BasicType::isSimpleType() const noexcept {
  return (m_name.size() == 1) &&
         (m_typeProperties ==
          BitField<TypeProperties>(TypeProperties::_default));
}

size_t BasicType::getHash() const {
  size_t h = this->Type::getHash();
  for (const auto &namePart : m_name) {
    h ^= (std::hash<std::string>{}(namePart) << 1);
  }
  return h;
}

TypePtr BasicType::getOwningType() const {
  return BasicType::make(m_name, m_typeProperties | TypeProperties::_owning);
}
TypePtr BasicType::getMutableType() const {
  return BasicType::make(m_name, m_typeProperties | TypeProperties::_mutable);
}
TypePtr BasicType::getValueType() const {
  return BasicType::make(m_name,
                         m_typeProperties & ~TypeProperties::_isPointer);
}
TypePtr BasicType::getUniquePointerType() const {
  return BasicType::make(m_name, m_typeProperties | TypeProperties::_isPointer);
}
TypePtr BasicType::getSharedPointerType() const {
  return BasicType::make(m_name, m_typeProperties | TypeProperties::_isPointer |
                                     TypeProperties::_isShared);
}

std::string BasicType::toTreeStr(size_t spaces) const {
  std::string typePrefix = (m_typeProperties & TypeProperties::_owning) ? (":")
                           : (m_typeProperties & TypeProperties::_mutable)
                               ? ("&")
                               : ("");

  if (m_typeProperties & TypeProperties::_isPointer) {
    typePrefix +=
        (m_typeProperties & TypeProperties::_isShared) ? ("{}") : ("<>");
  }

  return Formatter("Type<{}{}>", typePrefix,
                   Formatter<>::joinContainer('.', m_name));
}

TypePtr BasicType::t_implicit() {
  static auto instance = BasicType::make({"@implicit"});
  return instance;
}
TypePtr BasicType::t_void() {
  static auto instance = BasicType::make({"void"});
  return instance;
}
TypePtr BasicType::t_int8() {
  static auto instance = BasicType::make({"i8"});
  return instance;
}
TypePtr BasicType::t_int16() {
  static auto instance = BasicType::make({"i16"});
  return instance;
}
TypePtr BasicType::t_int32() {
  static auto instance = BasicType::make({"i32"});
  return instance;
}
TypePtr BasicType::t_int64() {
  static auto instance = BasicType::make({"i64"});
  return instance;
}
TypePtr BasicType::t_uint8() {
  static auto instance = BasicType::make({"u8"});
  return instance;
}
TypePtr BasicType::t_uint16() {
  static auto instance = BasicType::make({"u16"});
  return instance;
}
TypePtr BasicType::t_uint32() {
  static auto instance = BasicType::make({"u32"});
  return instance;
}
TypePtr BasicType::t_uint64() {
  static auto instance = BasicType::make({"u64"});
  return instance;
}
TypePtr BasicType::t_float() {
  static auto instance = BasicType::make({"float"});
  return instance;
}
TypePtr BasicType::t_double() {
  static auto instance = BasicType::make({"double"});
  return instance;
}
TypePtr BasicType::t_char() {
  static auto instance = BasicType::make({"char"});
  return instance;
}
TypePtr BasicType::t_string() {
  static auto instance =
      BasicType::make({"string"}, TypeProperties::_referenceType);
  return instance;
}
TypePtr BasicType::t_bool() {
  static auto instance = BasicType::make({"bool"});
  return instance;
}
};  // namespace rgl