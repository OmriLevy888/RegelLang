#pragma once
#include "codegen/values/symbols/types/TypeSymbolBase.hpp"

namespace rgl {
class FunctionTypeSymbol;
using FunctionTypeSymbolPtr = std::shared_ptr<FunctionTypeSymbol>;

class FunctionTypeSymbol : public TypeSymbolBase {
public:
  static FunctionTypeSymbolPtr make();

  virtual bool isFunctionType() const override { return true; }

  virtual bool operator==(TypeSymbolPtr other) const override;

  std::vector<TypeSymbolPtr> &paramTypes() { return m_paramTypes; }

private:
  TypeSymbolPtr m_retType;
  std::vector<TypeSymbolPtr> m_paramTypes;

  FunctionTypeSymbol();
};
}; // namespace rgl
