#pragma once
#include <memory>
#include <optional>

#include "common/Core.hpp"
#include "common/utils/BitField.hpp"
#include "parser/ast/constructs/ConstructNode.hpp"
#include "llvm/IR/Type.h"

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
  static void cleanNonBuiltinTypes();
  static std::string typeBankToString();

  bool operator==(TypePtr other) const;
  bool equals(TypePtr other) const;
  bool equals(const std::unique_ptr<Type> &other) const;

  virtual bool isSimpleType() const noexcept { return false; }

  virtual size_t getHash() const;

  virtual TypePtr getOwningType() const = 0;
  virtual TypePtr getMutableType() const = 0;
  virtual TypePtr getValueType() const = 0;
  virtual TypePtr getUniquePointerType() const = 0;
  virtual TypePtr getSharedPointerType() const = 0;

  virtual llvm::Type *toLLVMType() = 0;

  size_t getSizeBytes() const;
  size_t getSizeBits() const;
  static size_t getPointerSizeBytes() { return 8; }

protected:
  BitField<TypeProperties> m_typeProperties;
  size_t m_typeID;
  size_t m_sizeBits;
  mutable std::optional<size_t> m_cachedHash;

  Type(BitField<TypeProperties> properties, size_t sizeBits = 0,
       size_t typeID = 0)
      : m_typeProperties(properties), m_typeID(typeID), m_sizeBits(sizeBits),
        m_cachedHash(std::nullopt) {}
};
}; // namespace rgl

namespace std {
template <> struct hash<std::shared_ptr<rgl::Type>> {
  size_t operator()(std::shared_ptr<rgl::Type> type) const {
    return type->getHash();
  }
};

template <> struct hash<std::unique_ptr<rgl::Type>> {
  size_t operator()(const std::unique_ptr<rgl::Type> &type) const {
    return type->getHash();
  }
};

bool operator==(std::shared_ptr<rgl::Type> first,
                std::shared_ptr<rgl::Type> second);

bool operator==(const std::unique_ptr<rgl::Type> &first,
                const std::unique_ptr<rgl::Type> &second);
}; // namespace std
