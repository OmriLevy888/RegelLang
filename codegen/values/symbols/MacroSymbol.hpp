#pragma once
#include "codegen/values/symbols/CallableSymbol.hpp"

namespace rgl {
class MacroSymbolBase;
using MacroSymbolPtr = std::shared_ptr<MacroSymbolBase>;

class MacroSymbolBase : public CallableSymbol {
public:
  virtual bool isMacro() const override { return true; }
};

template<typename TMacro>
class MacroSymbol : public MacroSymbolBase {
public:
  static MacroSymbolPtr make(const std::function<TMacro> &macro) {

  }

private:
  std::function<TMacro> m_macro;

  MacroSymbol(const std::function<TMacro> &macro) :
    m_macro(macro) {}
};
} // namespace rgl
