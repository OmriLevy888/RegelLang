#include "codegen/ModuleTable.hpp"

namespace rgl {
const std::vector<std::string> MAIN_MODULE_NAME{"main"};

ModuleTable::ModuleTable(std::shared_ptr<llvm::LLVMContext> context)
    : m_context(context), m_modules({}) {
  createModule(MAIN_MODULE_NAME);
}

std::shared_ptr<ModuleTableEntry> ModuleTable::getMainEntry() {
  return getEntry(MAIN_MODULE_NAME);
}
std::shared_ptr<ModuleTableEntry>
ModuleTable::getEntry(const std::vector<std::string> &name) {
  return m_modules.get(name);
}

void ModuleTable::createModule(const std::vector<std::string> &name) {
  auto moduleTableEntry = ModuleTableEntry::make(name, m_context);
  m_modules.insert(name, moduleTableEntry);
}

std::string ModuleTable::toString() const { return "ModuelTable"; }
}; // namespace rgl
