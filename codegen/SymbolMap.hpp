#pragma once
#include "codegen/SymbolBase.hpp"
#include "common/ILoggable.hpp"
#include <unordered_map>
#include <vector>

namespace rgl {
class SymbolMap;
using SymbolMapPtr = std::shared_ptr<SymbolMap>;

class SymbolMap : public ILoggable {
public:
  SymbolMap();

  SymbolPtr get(const std::vector<std::string> &name);
  SymbolPtr insert(const std::vector<std::string> &name, SymbolPtr symbol);

  virtual std::string toString() const override;

private:
  std::unordered_map<std::string, SymbolMapPtr> m_maps;
  // TODO: replace with stack(?)
  SymbolPtr m_symbol;

  SymbolPtr get(std::vector<std::string>::const_iterator curr,
                std::vector<std::string>::const_iterator end);
  SymbolPtr insert(std::vector<std::string>::const_iterator curr,
                   std::vector<std::string>::const_iterator end,
                   SymbolPtr symbol);
};
}; // namespace rgl
