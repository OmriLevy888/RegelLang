#pragma once
#include "codegen/values/symbols/types/TypeSymbolBase.hpp"
#include "common/utils/BitField.hpp"

namespace rgl {
class ModifiedTypeSymbol;
using ModifiedTypeSymbolPtr = std::shared_ptr<ModifiedTypeSymbol>;

enum class TypeModifiers : uint64_t { _mutable = 1 };

class ModifiedTypeSymbol : public TypeSymbolBase {
public:
  static ModifiedTypeSymbolPtr make(TypeSymbolPtr typeSymbol,
                                    BitField<TypeModifiers> modifiers);

  virtual bool isModifiedType() const override { return true; }

  virtual bool isImplicitType() const override {
    return m_containedType->isImplicitType();
  }

private:
  BitField<TypeModifiers> m_modifiers;
  TypeSymbolPtr m_containedType;

  ModifiedTypeSymbol();
};
}; // namespace rgl
