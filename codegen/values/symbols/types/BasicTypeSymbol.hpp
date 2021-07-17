#pragma once
#include "codegen/values/symbols/FunctionSymbol.hpp"
#include "codegen/values/symbols/SymbolMap.hpp"
#include "codegen/values/symbols/types/TypeSymbolBase.hpp"

namespace rgl {
class BasicTypeSymbol;
using BasicTypeSymbolPtr = std::shared_ptr<BasicTypeSymbol>;

class BasicTypeSymbol : public TypeSymbolBase {
public:
  // TODO: implement this static BasicTypeSymbolPtr make(BasicTypePtr
  // basicType); for built in types such as `i32` and `bool`
  static BasicTypeSymbolPtr make(llvm::Type *llvmType,
                                 BitField<TypeTraits> traits,
                                 const std::string &name);

  virtual bool isBasicType() const override { return true; }

  virtual bool isImplicitType() const override {
    return m_fullyQualifiedName == "@implicit";
  }

  virtual bool operator==(TypeSymbolPtr other) const override;

private:
  std::string m_fullyQualifiedName;
  std::vector<FunctionSymbolPtr> m_ctors; // easy access to all ctors
  FunctionSymbolPtr m_dtor;               // easy access to dtor
  SymbolMapPtr m_members; // dtor and all ctors, fields and methods

  BasicTypeSymbol(llvm::Type *llvmType, BitField<TypeTraits> traits,
                  const std::string &name,
                  const std::vector<FunctionSymbolPtr> &ctors,
                  FunctionSymbolPtr dtor, SymbolMapPtr members);
};
}; // namespace rgl
