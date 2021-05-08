#include "codegen/SymbolMap.hpp"

namespace rgl {
SymbolMap::SymbolMap() : m_maps({}), m_symbol(nullptr) {}

SymbolPtr SymbolMap::get(const std::vector<std::string> &name) {
  return get(name.cbegin(), name.cend());
}
SymbolPtr SymbolMap::get(std::vector<std::string>::const_iterator curr,
                         std::vector<std::string>::const_iterator end) {
  if (curr == end) {
    return m_symbol;
  }

  auto nextMap = m_maps[*curr];
  if (nullptr == nextMap) {
    return nullptr;
  }
  return nextMap->get(curr + 1, end);
}

SymbolPtr SymbolMap::insert(const std::vector<std::string> &name,
                            SymbolPtr symbol) {
  return insert(name.cbegin(), name.cend(), symbol);
}
SymbolPtr SymbolMap::insert(std::vector<std::string>::const_iterator curr,
                            std::vector<std::string>::const_iterator end,
                            SymbolPtr symbol) {
  if (curr == end) {
    m_symbol = symbol;
    return symbol;
  }

  auto nextMap = m_maps[*curr];
  if (nullptr == nextMap) {
    nextMap = std::make_shared<SymbolMap>();
    m_maps[*curr] = nextMap;
  }
  return nextMap->insert(curr + 1, end, symbol);
}

std::string SymbolMap::toString() const { return "SymbolMap"; }
}; // namespace rgl
