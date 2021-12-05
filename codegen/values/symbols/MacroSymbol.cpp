#include "codegen/values/symbols/MacroSymbol.hpp"

namespace rgl {
BinOpMacroSymbol::BinOpMacroSymbol(const std::function<macro_t> &macro)
  : MacroSymbol(true), m_macro(macro) {}

MacroSymbolPtr BinOpMacroSymbol::make(const std::function<macro_t> &macro) {
  return MacroSymbolPtr(new BinOpMacroSymbol(macro));
}
} // namespace rgl
