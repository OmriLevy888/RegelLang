#include <functional>
#include <unordered_map>

#include "parser/ast/constructs/BasicType.hpp"
#include "parser/ast/constructs/FunctionType.hpp"

namespace rgl {
TypeProperties operator~(TypeProperties property) {
  using TUnderlying = typename std::underlying_type<TypeProperties>::type;
  return static_cast<TypeProperties>(~static_cast<TUnderlying>(property));
}

std::string typePropertiesToString(BitField<TypeProperties> properties) {
  return (properties & TypeProperties::_owning)    ? (":")
         : (properties & TypeProperties::_mutable) ? ("&")
                                                   : ("");
}

size_t Type::getHash() const { return std::hash<uint8_t>{}(m_typeProperties); }

std::unordered_map<std::unique_ptr<Type>, TypePtr> &getTypeBank() {
  static bool initialized = false;
  static std::unordered_map<std::unique_ptr<Type>, TypePtr> typeBank;
  if (!initialized) {
    // this has to happen first so that when BasicType::make is called and it
    // in turn calls getTypeBank, the if clause does not happen again
    initialized = true;

    BasicType::make({"void"});

    BasicType::make({"i8"});
    BasicType::make({"i16"});
    BasicType::make({"i32"});
    BasicType::make({"i64"});

    BasicType::make({"u8"});
    BasicType::make({"u16"});
    BasicType::make({"u32"});
    BasicType::make({"u64"});

    BasicType::make({"float"});
    BasicType::make({"double"});

    BasicType::make({"char"});
    BasicType::make({"string"}, TypeProperties::_referenceType);

    BasicType::make({"bool"});
  }
  return typeBank;
}

TypePtr BasicType::make(std::vector<std::string> &&name,
                        BitField<TypeProperties> properties) {
  auto &typeBank = getTypeBank();
  // in order to not expose the constructor, new is used since using the friend
  // keyword assumes that std::make_unique calls new inside of it. this call is
  // safe only because the unique pointer is assigned to a named variable right
  // after it is created(!)
  auto target =
      std::unique_ptr<Type>(new BasicType(std::move(name), properties));

  if (typeBank.cend() == typeBank.find(target)) {
    auto *innerPtr = dynamic_cast<BasicType *>(target.get());
    // see explanation above
    auto typePtr =
        std::shared_ptr<BasicType>(new BasicType(innerPtr->m_name, properties));
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }

  return typeBank.at(target);
}

TypePtr BasicType::make(const std::vector<std::string> &name,
                        BitField<TypeProperties> properties) {
  auto &typeBank = getTypeBank();
  // see explanation above
  auto target = std::unique_ptr<Type>(new BasicType(name, properties));

  if (typeBank.cend() == typeBank.find(target)) {
    auto typePtr = std::shared_ptr<BasicType>(new BasicType(name, properties));
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }

  return typeBank.at(target);
}

TypePtr FunctionType::make(std::vector<TypePtr> &&params, TypePtr retType,
                           BitField<TypeProperties> properties) {
  auto &typeBank = getTypeBank();
  // see explanation above
  auto target = std::unique_ptr<Type>(
      new FunctionType(std::move(params), retType, properties));

  if (typeBank.cend() == typeBank.find(target)) {
    auto *innerPtr = dynamic_cast<FunctionType *>(target.get());
    // see explanation above
    auto typePtr = std::shared_ptr<FunctionType>(
        new FunctionType(innerPtr->m_params, retType, properties));
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }

  return typeBank.at(target);
}

TypePtr FunctionType::make(const std::vector<TypePtr> &params, TypePtr retType,
                           BitField<TypeProperties> properties) {
  auto &typeBank = getTypeBank();
  // see explanation above
  auto target =
      std::unique_ptr<Type>(new FunctionType(params, retType, properties));

  if (typeBank.cend() == typeBank.find(target)) {
    auto typePtr = std::shared_ptr<FunctionType>(
        new FunctionType(params, retType, properties));
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }

  return typeBank.at(target);
}
};  // namespace rgl
