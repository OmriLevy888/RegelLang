#pragma once
#include "codegen/values/symbols/types/TypeSymbolBase.hpp"
#include "common/utils/BitField.hpp"

namespace rgl {
class ModifiedTypeSymbol;
using ModifiedTypeSymbolPtr = std::shared_ptr<ModifiedTypeSymbol>;

enum class TypeModifiers : uint64_t {};

class ModifiedTypeSymbol : public TypeSymbolBase {
public:
  static ModifiedTypeSymbolPtr make();

  virtual bool isModifiedType() const override { return true; }

private:
  BitField<TypeModifiers> m_modifiers;
  TypeSymbolPtr m_containedType;

  ModifiedTypeSymbol();
};
}; // namespace rgl
