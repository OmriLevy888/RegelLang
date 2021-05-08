#include "codegen/FunctionTable.hpp"

namespace rgl {
FunctionTableEntryPtr
FunctionTable::getEntry(const std::vector<std::string> &name) {
  return m_functions.get(name);
}
void FunctionTable::insertFunction(const std::vector<std::string> &name,
                                   FunctionTableEntryPtr function) {
  return m_functions.insert(name, function);
}

std::string FunctionTable::toString() const { return "FunctionTable"; }
}; // namespace rgl
