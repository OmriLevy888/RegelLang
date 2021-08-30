#pragma once
#include "codegen/values/symbols/CallableSymbol.hpp"

namespace rgl {
class MacroSymbol;
using MacroSymbolPtr = std::shared_ptr<MacroSymbol>;

class MacroSymbol : public CallableSymbol {
public:
  static MacroSymbolPtr make();

  virtual bool isMacro() const override { return true; }

private:
  std::function<void()> m_macro;

  MacroSymbol(const std::function<void()> &macro);
};
} // namespace rgl
