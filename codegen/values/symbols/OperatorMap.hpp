#pragma once
#include "codegen/values/symbols/SymbolMap.hpp"
#include "common/ILoggable.hpp"
#include <unordered_map>
#include <memory>

namespace rgl {
class OperatorMap : public ILoggable {
public:

  virtual std::string toString() const override;
  
private:
  std::unordered_map<TypeSymbolPtr, OperatorMapPtr> m_maps;
  CallableSymbolPtr m_symbol; 
};
};  // namespace rgl
