#pragma once
#include "common/ILoggable.hpp"
#include <memory>

namespace rgl {
class SymbolBase : public ILoggable {
public:
  // Clean when cleaning a namespace (SymbolMap)
  virtual void clean(){};

private:
};

using SymbolPtr = std::shared_ptr<SymbolBase>;
}; // namespace rgl
