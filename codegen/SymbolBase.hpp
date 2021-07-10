#pragma once
#include "common/ILoggable.hpp"
#include <memory>

namespace rgl {
class SymbolBase : public ILoggable {
public:
  // Clean when cleaning a namespace (SymbolMap)
  virtual void clean(){};

  virtual bool isFunction() const { return false; }
  virtual bool isNamespace() const { return false; }
  virtual bool isVariable() const { return false; }
  virtual bool isParameter() const { return false; }

private:
};

using SymbolPtr = std::shared_ptr<SymbolBase>;
}; // namespace rgl
