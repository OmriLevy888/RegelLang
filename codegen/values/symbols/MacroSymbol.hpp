#pragma once
#include "codegen/values/symbols/CallableSymbol.hpp"

namespace rgl {
class MacroSymbol;
using MacroSymbolPtr = std::shared_ptr<MacroSymbol>;

class MacroSymbol : public CallableSymbol {
public:
  using CallableSymbol::CallableSymbol;

  virtual bool isMacro() const override { return true; }
};

class BinOpMacroSymbol : public MacroSymbol {
public:
  using macro_t = ValuePtr(ValuePtr, ValuePtr);

  static MacroSymbolPtr make(const std::function<macro_t> &macro);

private:
  std::function<macro_t> m_macro;
  //std::function<TMacro> m_macro;

  BinOpMacroSymbol(const std::function<macro_t> &macro);
};
} // namespace rgl
