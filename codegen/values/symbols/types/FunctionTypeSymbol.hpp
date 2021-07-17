#pragma once
#include "codegen/values/symbols/types/TypeSymbolBase.hpp"

namespace rgl {
class FunctionTypeSymbol;
using FunctionTypeSymbolPtr = std::shared_ptr<FunctionTypeSymbol>;

class FunctionTypeSymbol : public TypeSymbolBase {
public:
  static FunctionTypeSymbolPtr make(const TypeNodePtr &typeNode);
  static FunctionTypeSymbolPtr make(TypeSymbolPtr retType,
                                    std::vector<TypeSymbolPtr> &&params);

  virtual bool isFunctionType() const override { return true; }

  virtual bool operator==(TypeSymbolPtr other) const override;

  TypeSymbolPtr retType() const { return m_retType; }
  std::vector<TypeSymbolPtr> &paramTypes() { return m_paramTypes; }

private:
  TypeSymbolPtr m_retType;
  std::vector<TypeSymbolPtr> m_paramTypes;

  FunctionTypeSymbol(llvm::Type *llvmType, BitField<TypeTraits> traits,
                     TypeSymbolPtr retType,
                     std::vector<TypeSymbolPtr> &&paramTypes);
};
}; // namespace rgl
