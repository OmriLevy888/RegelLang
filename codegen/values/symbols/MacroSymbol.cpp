#include "codegen/values/symbols/MacroSymbol.hpp"

namespace rgl {
MacroSymbol::MacroSymbol(const std::function<void()> &macro) 
  : CallableSymbol(true), m_macro(macro) {}

} // namespace rgl
