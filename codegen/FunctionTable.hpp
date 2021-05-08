#pragma once
#include "codegen/FunctionTableEntry.hpp"
#include "common/ILoggable.hpp"
#include <memory>

namespace rgl {
class FunctionTable : public ILoggable {
public:
  FunctionTableEntryPtr getEntry(const std::vector<std::string> &name);
  void insertFunction(const std::vector<std::string> &name,
                      FunctionTableEntryPtr function);

  virtual std::string toString() const override;

private:
  FunctionTableMap m_functions;
};
}; // namespace rgl
