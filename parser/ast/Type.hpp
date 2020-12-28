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
static TypePtr makeType(std::vector<std::string> &&name,
                        bool isReference = false);
static TypePtr makeType(const std::vector<std::string> &name,
                        bool isReference = false);

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

  static TypePtr t_void() {
    static TypePtr instance = std::make_shared<Type>("void");
    return instance;
  }
  static TypePtr t_int8() {
    static TypePtr instance = std::make_shared<Type>("i8");
    return instance;
  }
  static TypePtr t_int16() {
    static TypePtr instance = std::make_shared<Type>("i16");
    return instance;
  }
  static TypePtr t_int32() {
    static TypePtr instance = std::make_shared<Type>("i32");
    return instance;
  }
  static TypePtr t_int64() {
    static TypePtr instance = std::make_shared<Type>("i64");
    return instance;
  }
  static TypePtr t_uint8() {
    static TypePtr instance = std::make_shared<Type>("u8");
    return instance;
  }
  static TypePtr t_uint16() {
    static TypePtr instance = std::make_shared<Type>("u16");
    return instance;
  }
  static TypePtr t_uint32() {
    static TypePtr instance = std::make_shared<Type>("u32");
    return instance;
  }
  static TypePtr t_uint64() {
    static TypePtr instance = std::make_shared<Type>("u64");
    return instance;
  }
  static TypePtr t_float() {
    static TypePtr instance = std::make_shared<Type>("float");
    return instance;
  }
  static TypePtr t_double() {
    static TypePtr instance = std::make_shared<Type>("double");
    return instance;
  }
  static TypePtr t_char() {
    static TypePtr instance = std::make_shared<Type>("char");
    return instance;
  }
  static TypePtr t_string() {
    static TypePtr instance = std::make_shared<Type>("string");
    return instance;
  }
  static TypePtr t_bool() {
    static TypePtr instance = std::make_shared<Type>("bool");
    return instance;
  }
};
}; // namespace rgl

namespace std {
template <> struct hash<rgl::Type> {
  size_t operator()(const rgl::Type &type) const noexcept {
    return type.getHash();
  }
};
}; // namespace std

namespace rgl {
std::unordered_map<Type, std::shared_ptr<Type>> typeBank{
    {*Type::t_void(), Type::t_void()},
    {*Type::t_int8(), Type::t_int8()},

    {*Type::t_int8(), Type::t_int8()},
    {*Type::t_int16(), Type::t_int16()},
    {*Type::t_int32(), Type::t_int32()},
    {*Type::t_int64(), Type::t_int64()},

    {*Type::t_uint8(), Type::t_uint8()},
    {*Type::t_uint16(), Type::t_uint16()},
    {*Type::t_uint32(), Type::t_uint32()},
    {*Type::t_uint64(), Type::t_uint64()},

    {*Type::t_float(), Type::t_float()},
    {*Type::t_double(), Type::t_double()},

    {*Type::t_char(), Type::t_char()},
    {*Type::t_string(), Type::t_string()},

    {*Type::t_bool(), Type::t_bool()},
};

TypePtr makeType(std::vector<std::string> &&name, bool isReference) {
  Type target{std::move(name), isReference};
  if (typeBank.cend() == typeBank.find(target)) {
    auto typePtr = std::make_shared<Type>(target.m_name, isReference);
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }
  return typeBank[target];
}

TypePtr makeType(const std::vector<std::string> &name, bool isReference) {
  Type target{name, isReference};
  if (typeBank.cend() == typeBank.find(target)) {
    auto typePtr = std::make_shared<Type>(target.m_name, isReference);
    return typePtr;
  }
  return typeBank[target];
}
}; // namespace rgl
