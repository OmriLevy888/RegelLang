#pragma once
#include "common/BitField.hpp"
#include "common/Core.hpp"
#include "common/ILoggable.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace rgl {
enum class TypeProperties : uint8_t {
  _default = 0b00000000,
  _mutable = 0b00000001,
  _referenceType = 0b00000010,
  _owning = 0b00000100,
  _isPointer = 0b00001000,
  _isShared = 0b00010000,
};

TypeProperties operator~(TypeProperties property);

class Type;
using TypePtr = std::shared_ptr<Type>;
TypePtr makeType(std::vector<std::string> &&name,
                 TypeProperties properties = TypeProperties::_default);
TypePtr makeType(const std::vector<std::string> &name,
                 TypeProperties properties = TypeProperties::_default);
extern std::unordered_map<Type, TypePtr> typeBank;

class Type : public ILoggable {
public:
  std::vector<std::string> m_name;
  BitField<TypeProperties> m_typeProperties;

  Type()
      : m_name(t_void()->m_name), m_typeProperties(t_void()->m_typeProperties) {
  }

  Type(std::vector<std::string> &&name,
       TypeProperties properties = TypeProperties::_default)
      : m_name(std::move(name)), m_typeProperties(properties) {}
  Type(const std::vector<std::string> &name,
       TypeProperties properties = TypeProperties::_default)
      : m_name(name), m_typeProperties(properties) {}
  Type(const std::string &name,
       TypeProperties properties = TypeProperties::_default)
      : m_name({name}), m_typeProperties(properties) {}

  TypePtr getOwningType() const {
    return makeType(m_name, m_typeProperties | TypeProperties::_owning);
  }
  TypePtr getMutableType() const {
    return makeType(m_name, m_typeProperties | TypeProperties::_mutable);
  }
  TypePtr getValueType() const {
    return makeType(m_name, m_typeProperties & ~TypeProperties::_isPointer);
  }
  TypePtr getUniquePointerType() const {
    return makeType(m_name, m_typeProperties | TypeProperties::_isPointer);
  }
  TypePtr getSharedPointerType() const {
    return makeType(m_name, m_typeProperties | TypeProperties::_isPointer |
                                TypeProperties::_isShared);
  }

  bool isSimpleType() const noexcept {
    return (1 == m_name.size()) &&
           (TypeProperties::_default == m_typeProperties);
  }

  size_t getHash() const {
    size_t h = std::hash<std::string>{}(m_name[0]);
    for (size_t idx = 1; idx < m_name.size(); idx++) {
      h ^= std::hash<std::string>{}(m_name[idx]) << 1;
    }
    return h ^ (std::hash<uint8_t>{}(m_typeProperties));
  }

  bool operator==(const Type &other) const {
    if (m_name.size() != other.m_name.size()) {
      return false;
    }

    if (m_typeProperties != other.m_typeProperties) {
      return false;
    }

    for (auto it = m_name.cbegin(), otherIt = other.m_name.cbegin();
         it != m_name.cend(); it++, otherIt++) {
      if (*it != *otherIt) {
        return false;
      }
    }

    return true;
  }

  virtual std::string toString() const override;

  static TypePtr t_implicit();
  static TypePtr t_void();
  static TypePtr t_int8();
  static TypePtr t_int16();
  static TypePtr t_int32();
  static TypePtr t_int64();
  static TypePtr t_uint8();
  static TypePtr t_uint16();
  static TypePtr t_uint32();
  static TypePtr t_uint64();
  static TypePtr t_float();
  static TypePtr t_double();
  static TypePtr t_char();
  static TypePtr t_string();
  static TypePtr t_bool();
};
}; // namespace rgl

namespace std {
template <> struct hash<rgl::Type> {
  size_t operator()(const rgl::Type &type) const noexcept {
    return type.getHash();
  }
};
}; // namespace std
