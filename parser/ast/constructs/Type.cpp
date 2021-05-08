#include <functional>
#include <unordered_map>

#include "codegen/Context.hpp"
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

size_t Type::getSizeBytes() const { return getSizeBits() / 8; }
size_t Type::getSizeBits() const { return m_sizeBits; }

size_t Type::getHash() const { return std::hash<uint8_t>{}(m_typeProperties); }

class TypeBank {
public:
  static auto &get() {
    static bool initialized = false;
    static std::unordered_map<std::unique_ptr<Type>, TypePtr> typeBank{};
    if (!initialized) {
      // this has to happen first so that when BasicType::make is called and it
      // in turn calls TypeBank::get, the if clause does not happen again
      initialized = true;

      TypeBank::initBuiltinTypes();
    }

    return typeBank;
  }

  static void initBuiltinTypes() {
    BasicType::make({"void"}, TypeProperties::_default, 0,
                    llvm::Type::getVoidTy(*Context::llvmContext()));

    BasicType::make({"i8"}, TypeProperties::_default, 8,
                    llvm::Type::getInt8Ty(*Context::llvmContext()));
    BasicType::make({"i16"}, TypeProperties::_default, 16,
                    llvm::Type::getInt16Ty(*Context::llvmContext()));
    BasicType::make({"i32"}, TypeProperties::_default, 32,
                    llvm::Type::getInt32Ty(*Context::llvmContext()));
    BasicType::make({"i64"}, TypeProperties::_default, 64,
                    llvm::Type::getInt64Ty(*Context::llvmContext()));

    BasicType::make({"u8"}, TypeProperties::_default, 8,
                    llvm::Type::getInt8Ty(*Context::llvmContext()));
    BasicType::make({"u16"}, TypeProperties::_default, 16,
                    llvm::Type::getInt16Ty(*Context::llvmContext()));
    BasicType::make({"u32"}, TypeProperties::_default, 32,
                    llvm::Type::getInt32Ty(*Context::llvmContext()));
    BasicType::make({"u64"}, TypeProperties::_default, 64,
                    llvm::Type::getInt64Ty(*Context::llvmContext()));

    BasicType::make({"float"}, TypeProperties::_default, 32,
                    llvm::Type::getFloatTy(*Context::llvmContext()));
    BasicType::make({"double"}, TypeProperties::_default, 64,
                    llvm::Type::getDoubleTy(*Context::llvmContext()));

    BasicType::make({"char"}, TypeProperties::_default, 8,
                    llvm::Type::getInt8Ty(*Context::llvmContext()));
    // TODO: figure out size of string class and llvm type
    BasicType::make({"string"}, TypeProperties::_referenceType);

    BasicType::make({"bool"}, TypeProperties::_default, 1,
                    llvm::Type::getInt1Ty(*Context::llvmContext()));
  }
};

void Type::cleanNonBuiltinTypes() {
  TypeBank::get().clear();
  TypeBank::initBuiltinTypes();
}

std::string Type::typeBankToString() {
  std::string ret = "{\n";
  for (const auto &[key, value] : TypeBank::get()) {
    ret += Formatter("    {} => {},\n", key->toString(), value->toString());
  }
  return ret + "}";
}

TypePtr BasicType::make(std::vector<std::string> &&name,
                        BitField<TypeProperties> properties, size_t sizeBits,
                        llvm::Type *llvmType) {
  auto &typeBank = TypeBank::get();
  // in order to not expose the constructor, new is used since using the friend
  // keyword assumes that std::make_unique calls new inside of it. this call is
  // safe only because the unique pointer is assigned to a named variable right
  // after it is created(!)
  auto target = std::unique_ptr<Type>(
      new BasicType(std::move(name), properties, sizeBits, llvmType));

  if (typeBank.cend() == typeBank.find(target)) {
    auto *innerPtr = dynamic_cast<BasicType *>(target.get());
    // see explanation above
    auto typePtr = std::shared_ptr<BasicType>(
        new BasicType(innerPtr->m_name, properties, sizeBits, llvmType));
    typePtr->m_typeID = typeBank.size() + 1;
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }

  return typeBank.at(target);
}

TypePtr BasicType::make(const std::vector<std::string> &name,
                        BitField<TypeProperties> properties, size_t sizeBits,
                        llvm::Type *llvmType) {
  auto &typeBank = TypeBank::get();
  // see explanation above
  auto target = std::unique_ptr<Type>(
      new BasicType(name, properties, sizeBits, llvmType));

  if (typeBank.cend() == typeBank.find(target)) {
    auto typePtr = std::shared_ptr<BasicType>(
        new BasicType(name, properties, sizeBits, llvmType));
    typePtr->m_typeID = typeBank.size() + 1;
    typeBank[std::move(target)] = typePtr;
    return typePtr;
  }

  return typeBank.at(target);
}

FunctionTypePtr FunctionType::make(std::vector<TypePtr> &&params,
                                   TypePtr retType,
                                   BitField<TypeProperties> properties) {
  auto &typeBank = TypeBank::get();
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

  return std::dynamic_pointer_cast<FunctionType>(typeBank.at(target));
}

FunctionTypePtr FunctionType::make(const std::vector<TypePtr> &params,
                                   TypePtr retType,
                                   BitField<TypeProperties> properties) {
  auto &typeBank = TypeBank::get();
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

  return std::dynamic_pointer_cast<FunctionType>(typeBank.at(target));
}
}; // namespace rgl
