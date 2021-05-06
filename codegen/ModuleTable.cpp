#include "codegen/ModuleTable.hpp"

namespace rgl {
ModuleTable::ModuleTable(std::shared_ptr<llvm::LLVMContext> context)
    : m_context(context), m_modules({}) {}

std::shared_ptr<ModuleTableEntry> ModuleTable::getMainEntry() {
  return getEntry("main");
}

std::shared_ptr<ModuleTableEntry>
ModuleTable::getEntry(const std::string &name) {
  auto ret = m_modules[name];
  if (nullptr == ret) {
    ret = std::make_shared<ModuleTableEntry>(name, m_context);
    m_modules[name] = ret;
  }
  return ret;
}

std::string ModuleTable::toString() const { return "ModuelTable"; }
}; // namespace rgl
