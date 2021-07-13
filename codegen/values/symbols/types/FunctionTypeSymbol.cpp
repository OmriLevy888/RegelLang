#include "codegen/values/symbols/types/FunctionTypeSymbol.hpp"

namespace rgl {
FunctionTypeSymbolPtr FunctionTypeSymbol::make() { return nullptr; }

bool FunctionTypeSymbol::operator==(TypeSymbolPtr other) const { return false; }
}; // namespace rgl
