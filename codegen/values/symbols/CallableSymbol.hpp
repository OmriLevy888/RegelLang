#pragma once
#include "codegen/values/symbols/SymbolBase.hpp"

namespace rgl {
class CallableSymbol;
using CallableSymbolPtr = std::shared_ptr<CallableSymbol>;

class CallableSymbol : public SymbolBase {
public:
  using SymbolBase::SymbolBase;
  
  virtual bool isCallable() const override { return true; }

private:
};
} // namespace rgl
