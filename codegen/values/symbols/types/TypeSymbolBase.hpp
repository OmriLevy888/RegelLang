#pragma once
#include "codegen/values/symbols/SymbolBase.hpp"
#include "common/utils/BitField.hpp"
#include "parser/ast/constructs/TypeNodeBase.hpp"

namespace rgl {
class TypeSymbolBase;
using TypeSymbolPtr = std::shared_ptr<TypeSymbolBase>;

enum class TypeTraits : uint64_t { TriviallyCopiable = 1 };

class TypeSymbolBase : public SymbolBase {
public:
  static TypeSymbolPtr make(TypeNodePtr typeNode);

  virtual bool isType() const override { return true; }
  virtual bool isBasicType() const { return false; }
  virtual bool isFunctionType() const { return false; }
  virtual bool isArrayType() const { return false; }
  virtual bool isModifiedType() const { return false; }
  virtual bool isConst() const { return false; }
  virtual bool isImplicitType() const { return false; }

  virtual bool operator==(TypeSymbolPtr other) const = 0;

  virtual size_t getSizeBits() const {
    return llvmType()->getScalarSizeInBits();
  }

protected:
  BitField<TypeTraits> m_traits;

  TypeSymbolBase(llvm::Type *llvmType, BitField<TypeTraits> traits)
      : SymbolBase(llvmType), m_traits(traits) {}
};
}; // namespace rgl
