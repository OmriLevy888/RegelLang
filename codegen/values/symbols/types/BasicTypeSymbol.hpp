#pragma once
#include "codegen/values/symbols/FunctionSymbol.hpp"
#include "codegen/values/symbols/SymbolMap.hpp"
#include "codegen/types/TypeSymbolBase.hpp"

namespace rgl {
class BasicTypeSymbol;
using BasicTypeSymbolPtr = std::shared_ptr<BasicTypeSymbol>;

class BasicTypeSymbol : public TypeSymbolBase {
public:
  static BasicTypeSymbolPtr make();

  virtual bool isBasicType() const override { return true; }

private:
  std::vector<FunctionSymbolPtr> m_ctors; // easy access to all ctors
  FunctionSymbolPtr m_dtor;               // easy access to dtor
  SymbolMapPtr m_members; // dtor and all ctors, fields and methods

  BasicTypeSymbol();
};
}; // namespace rgl
