#include "codegen/values/symbols/types/BasicTypeSymbol.hpp"

namespace rgl {
BasicTypeSymbol::BasicTypeSymbol(llvm::Type *llvmType,
                                 BitField<TypeTraits> traits,
                                 const std::string &name,
                                 const std::vector<FunctionSymbolPtr> &ctors,
                                 FunctionSymbolPtr dtor, SymbolMapPtr members)
    : TypeSymbolBase(llvmType, traits), m_fullyQualifiedName(name),
      m_ctors(ctors), m_dtor(dtor), m_members(members) {}

BasicTypeSymbolPtr BasicTypeSymbol::make(llvm::Type *llvmType,
                                         BitField<TypeTraits> traits,
                                         const std::string &name) {
  std::vector<FunctionSymbolPtr> ctors{};
  FunctionSymbolPtr dtor = nullptr;
  SymbolMapPtr members = nullptr;
  return BasicTypeSymbolPtr(
      new BasicTypeSymbol(llvmType, traits, name, ctors, dtor, members));
}

bool BasicTypeSymbol::operator==(TypeSymbolPtr other) const {
  if (other.get() == this) {
    return true;
  }

  if (!other->isBasicType()) {
    return false;
  }
  return m_fullyQualifiedName ==
         std::dynamic_pointer_cast<BasicTypeSymbol>(other)
             ->m_fullyQualifiedName;
}
}; // namespace rgl
