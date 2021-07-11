#pragma once
#include "codegen/values/symbols/types/TypeSymbolBase.hpp"

namespace rgl {
class FunctionTypeSymbol;
using FunctionTypeSymbolPtr = std::shared_ptr<FunctionTypeSymbol>;

class FunctionTypeSymbol : public TypeSymbolBase {
public:
  static FunctionTypeSymbolPtr make();

  virtual bool isFunctionType() const override { return true; }

private:
  TypeSymbolPtr m_retType;
  std::vector<TypeSymbolPtr> m_paramTypes;

  FunctionTypeSymbol();
};
}; // namespace rgl
