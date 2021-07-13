#pragma once
#include "common/Core.hpp"
#include "common/utils/BitField.hpp"
#include "parser/ast/constructs/ConstructNode.hpp"

namespace rgl {
enum class TypeProperties : uint8_t {
  _default = 0b00000000,
  _mutable = 0b00000001,
  _owning = 0b00000010,
  _isPointer = 0b00000100,
  _isShared = 0b00001000,
  _isFunction = 0b00010000
};

TypeProperties operator~(TypeProperties property);
std::string typePropertiesToString(BitField<TypeProperties> properties);

class TypeNodeBase;
using TypeNodePtr = std::unique_ptr<TypeNodeBase>;

class TypeNodeBase : public ConstructNode {
public:
  virtual bool isBasicType() const noexcept { return false; }
  virtual bool isImplicitType() const noexcept { return false; }
  virtual bool isSimpleType() const noexcept {
    return false;
  } /* no modifiers, only basic identifier */
  virtual bool isFunctionType() const noexcept { return false; }

  virtual TypeNodePtr getOwningType() const = 0;
  virtual TypeNodePtr getMutableType() const = 0;
  virtual TypeNodePtr getValueType() const = 0;
  virtual TypeNodePtr getUniquePointerType() const = 0;
  virtual TypeNodePtr getSharedPointerType() const = 0;

  bool isMutable() const noexcept;
  bool isConst() const noexcept;
  bool isOwning() const noexcept;
  bool isPointer() const noexcept;
  bool isUniquePointer() const noexcept;
  bool isSharedPointer() const noexcept;

  virtual TypeNodePtr clone() const = 0;

  BitField<TypeProperties> &typeProperties() { return m_typeProperties; }

protected:
  BitField<TypeProperties> m_typeProperties;

  TypeNodeBase(BitField<TypeProperties> properties)
      : m_typeProperties(properties) {}
};
}; // namespace rgl
