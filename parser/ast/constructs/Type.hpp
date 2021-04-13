#pragma once
#include <memory>

#include "common/Core.hpp"
#include "common/utils/BitField.hpp"
#include "parser/ast/constructs/ConstructNode.hpp"

namespace rgl {
enum class TypeProperties : uint8_t {
  _default = 0b00000000,
  _mutable = 0b00000001,
  _referenceType = 0b00000010,
  _owning = 0b00000100,
  _isPointer = 0b00001000,
  _isShared = 0b00010000,
  _isFunction = 0b00100000
};

TypeProperties operator~(TypeProperties property);
std::string typePropertiesToString(BitField<TypeProperties> properties);

class Type;
using TypePtr = std::shared_ptr<Type>;

class Type : public ConstructNode {
public:
  virtual bool equals(TypePtr other) const = 0;

  virtual bool isSimpleType() const noexcept { return false; }

  virtual size_t getHash() const;

  virtual TypePtr getOwningType() const = 0;
  virtual TypePtr getMutableType() const = 0;
  virtual TypePtr getValueType() const = 0;
  virtual TypePtr getUniquePointerType() const = 0;
  virtual TypePtr getSharedPointerType() const = 0;

protected:
  BitField<TypeProperties> m_typeProperties;

  Type(BitField<TypeProperties> properties) : m_typeProperties(properties) {}
};
}; // namespace rgl

namespace std {
template <> struct hash<std::unique_ptr<rgl::Type>> {
  size_t operator()(const std::unique_ptr<rgl::Type> &type) const noexcept {
    return type->getHash();
  }
};

template <> struct hash<std::shared_ptr<rgl::Type>> {
  size_t operator()(const std::shared_ptr<rgl::Type> &type) const noexcept {
    return type->getHash();
  }
};
}; // namespace std
