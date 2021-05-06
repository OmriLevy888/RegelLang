#pragma once
#include "codegen/ModuleTableEntry.hpp"
#include "common/ILoggable.hpp"
#include "unordered_map"

namespace rgl {
class ModuleTable : public ILoggable {
public:
  ModuleTable(std::shared_ptr<llvm::LLVMContext> context);

  virtual std::string toString() const override;

  std::shared_ptr<ModuleTableEntry> getMainEntry();
  std::shared_ptr<ModuleTableEntry> getEntry(const std::string &name);

private:
  std::shared_ptr<llvm::LLVMContext> m_context;
  std::unordered_map<std::string, std::shared_ptr<ModuleTableEntry>> m_modules;
};
}; // namespace rgl
