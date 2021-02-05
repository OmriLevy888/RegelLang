#include "parser/ast/Type.hpp"

namespace rgl {
TypeProperties operator~(TypeProperties property) {
  using TUnderlying = typename std::underlying_type<TypeProperties>::type;
  return static_cast<TypeProperties>(~static_cast<TUnderlying>(property));
}

TypePtr Type::t_implicit() {
  static TypePtr instance = std::make_shared<Type>("implicit");
  return instance;
}
TypePtr Type::t_void() {
  static TypePtr instance = std::make_shared<Type>("void");
  return instance;
}
TypePtr Type::t_int8() {
  static TypePtr instance = std::make_shared<Type>("i8");
  return instance;
}
TypePtr Type::t_int16() {
  static TypePtr instance = std::make_shared<Type>("i16");
  return instance;
}
TypePtr Type::t_int32() {
  static TypePtr instance = std::make_shared<Type>("i32");
  return instance;
}
TypePtr Type::t_int64() {
  static TypePtr instance = std::make_shared<Type>("i64");
  return instance;
}
TypePtr Type::t_uint8() {
  static TypePtr instance = std::make_shared<Type>("u8");
  return instance;
}
TypePtr Type::t_uint16() {
  static TypePtr instance = std::make_shared<Type>("u16");
  return instance;
}
TypePtr Type::t_uint32() {
  static TypePtr instance = std::make_shared<Type>("u32");
  return instance;
}
TypePtr Type::t_uint64() {
  static TypePtr instance = std::make_shared<Type>("u64");
  return instance;
}
TypePtr Type::t_float() {
  static TypePtr instance = std::make_shared<Type>("float");
  return instance;
}
TypePtr Type::t_double() {
  static TypePtr instance = std::make_shared<Type>("double");
  return instance;
}
TypePtr Type::t_char() {
  static TypePtr instance = std::make_shared<Type>("char");
  return instance;
}
TypePtr Type::t_string() {
  static TypePtr instance =
      std::make_shared<Type>("string", TypeProperties::_referenceType);
  return instance;
}
TypePtr Type::t_bool() {
  static TypePtr instance = std::make_shared<Type>("bool");
  return instance;
}

std::unordered_map<Type, TypePtr> typeBank{
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

TypePtr makeType(std::vector<std::string> &&name, TypeProperties properties) {
  Type target{std::move(name), properties};
  if (typeBank.cend() == typeBank.find(target)) {
    auto typePtr = std::make_shared<Type>(target.m_name, properties);
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }
  return typeBank[target];
}

TypePtr makeType(const std::vector<std::string> &name,
                 TypeProperties properties) {
  Type target{name, properties};
  if (typeBank.cend() == typeBank.find(target)) {
    auto typePtr = std::make_shared<Type>(target.m_name, properties);
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }
  return typeBank[target];
}
}; // namespace rgl
