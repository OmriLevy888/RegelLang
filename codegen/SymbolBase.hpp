#pragma once
#include "common/ILoggable.hpp"
#include <memory>

namespace rgl {
class SymbolBase : public ILoggable {
public:
private:
};

using SymbolPtr = std::shared_ptr<SymbolBase>;
}; // namespace rgl
