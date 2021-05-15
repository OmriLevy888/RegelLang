#include "parser/ast/constructs/BasicType.hpp"

namespace rgl {
bool BasicType::isImplicitType() const {
  return m_name.size() == 1 && m_name[0] == "@implicit";
}

bool BasicType::isSimpleType() const noexcept {
  return (m_name.size() == 1) &&
         (m_typeProperties ==
          BitField<TypeProperties>(TypeProperties::_default));
}

size_t BasicType::getHash() const {
  if (!m_cachedHash.has_value()) {
    size_t h = this->Type::getHash();
    for (const auto &namePart : m_name) {
      h ^= (std::hash<std::string>{}(namePart) << 1);
    }
    m_cachedHash = h;
  }

  return m_cachedHash.value();
}

TypePtr BasicType::getOwningType() const {
  return BasicType::make(m_name, m_typeProperties | TypeProperties::_owning,
                         m_sizeBits, m_llvmType);
}
TypePtr BasicType::getMutableType() const {
  return BasicType::make(m_name, m_typeProperties | TypeProperties::_mutable,
                         m_sizeBits, m_llvmType);
}
TypePtr BasicType::getValueType() const {
  return BasicType::make(m_name, m_typeProperties & ~TypeProperties::_isPointer,
                         m_sizeBits, m_llvmType);
}
// TODO: fix these casts
TypePtr BasicType::getUniquePointerType() const {
  return BasicType::make(
      m_name, m_typeProperties | TypeProperties::_isPointer, m_sizeBits,
      reinterpret_cast<llvm::Type *>(m_llvmType->getPointerTo()));
}
TypePtr BasicType::getSharedPointerType() const {
  return BasicType::make(
      m_name,
      m_typeProperties | TypeProperties::_isPointer | TypeProperties::_isShared,
      m_sizeBits, reinterpret_cast<llvm::Type *>(m_llvmType->getPointerTo()));
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

  return Formatter("Type<{}{} :: {}>", typePrefix,
                   Formatter<>::joinContainer('.', m_name), getSizeBytes());
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

llvm::Type *BasicType::toLLVMType() { return m_llvmType; }
}; // namespace rgl
