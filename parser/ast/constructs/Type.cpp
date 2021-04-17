#include <functional>
#include <unordered_map>

#include "parser/ast/constructs/BasicType.hpp"
#include "parser/ast/constructs/FunctionType.hpp"

namespace std {
bool operator==(std::shared_ptr<rgl::Type> first,
                std::shared_ptr<rgl::Type> second) {
  return first->equals(second);
}

bool operator==(const std::unique_ptr<rgl::Type> &first,
                const std::unique_ptr<rgl::Type> &second) {
  return first->equals(second);
}
} // namespace std

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

bool Type::operator==(TypePtr other) const { return this->equals(other); }

bool Type::equals(TypePtr other) const {
  if (this->m_typeID == other->m_typeID) {
    return true;
  }

  if (this->getHash() != other->getHash()) {
    return false;
  }

  const std::string thisRepr = this->toString();
  const std::string otherRepr = other->toString();

  if (thisRepr.length() != otherRepr.length()) {
    return false;
  }
  return thisRepr == otherRepr;
}

bool Type::equals(const std::unique_ptr<Type> &other) const {
  if (this->m_typeID == other->m_typeID) {
    return true;
  }

  if (this->getHash() != other->getHash()) {
    return false;
  }

  const std::string thisRepr = this->toString();
  const std::string otherRepr = other->toString();

  if (thisRepr.length() != otherRepr.length()) {
    return false;
  }
  return thisRepr == otherRepr;
}

size_t Type::getHash() const { return std::hash<uint8_t>{}(m_typeProperties); }

std::unordered_map<std::unique_ptr<Type>, TypePtr> &
getTypeBank(bool forceInitialize = false) {
  static bool initialized = false;
  static std::unordered_map<std::unique_ptr<Type>, TypePtr> typeBank{};
  if (!initialized || forceInitialize) {
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

std::string typeBankToString() {
  std::string ret = "{\n";
  for (const auto &[key, value] : getTypeBank()) {
    ret += Formatter("    {} => {},\n", key->toString(), value->toString());
  }
  return ret + "}";
}

void clearTypeBank() { getTypeBank().clear(); }

void initTypeBank() { getTypeBank(true); }

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
    typePtr->m_typeID = typeBank.size() + 1;
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
    typePtr->m_typeID = typeBank.size() + 1;
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
    typePtr->m_typeID = typeBank.size() + 1;
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
    typePtr->m_typeID = typeBank.size() + 1;
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }

  return typeBank.at(target);
}
}; // namespace rgl
