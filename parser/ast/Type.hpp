#pragma once
#include "common/Core.hpp"
#include "common/ILoggable.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace rgl {
class Type;
using TypePtr = std::shared_ptr<Type>;
TypePtr makeType(std::vector<std::string> &&name, bool isReference = false);
TypePtr makeType(const std::vector<std::string> &name,
                 bool isReference = false);
extern std::unordered_map<Type, TypePtr> typeBank;

class Type : public ILoggable {
public:
  std::vector<std::string> m_name;
  bool m_isReference;

  Type() : m_name(t_void()->m_name), m_isReference(t_void()->m_isReference) {}

  Type(std::vector<std::string> &&name, bool isReference = false)
      : m_name(std::move(name)), m_isReference(isReference) {}
  Type(const std::vector<std::string> &name, bool isReference = false)
      : m_name(name), m_isReference(isReference) {}
  Type(const std::string &name, bool isReference = false)
      : m_name({name}), m_isReference(isReference) {}

  TypePtr getReferenceType() { return makeType(m_name, true); }
  TypePtr getValueType() { return makeType(m_name, false); }

  size_t getHash() const {
    size_t h = std::hash<std::string>{}(m_name[0]);
    for (size_t idx = 1; idx < m_name.size(); idx++) {
      h ^= std::hash<std::string>{}(m_name[idx]) << 1;
    }
    return h ^ (std::hash<bool>{}(m_isReference));
  }

  bool operator==(const Type &other) const {
    if (m_name.size() != other.m_name.size()) {
      return false;
    }

    if (m_isReference != other.m_isReference) {
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

  std::string toString() const override {
    std::string referencePrefix = (m_isReference) ? ("&") : ("");
    return Formatter("Type<{}{}>", referencePrefix,
                     Formatter<>::joinContainer('.', m_name));
  }

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
