#include "codegen/ModuleTableEntry.hpp"
#include "common/Formatter.hpp"
#include "llvm/Support/raw_ostream.h"

namespace rgl {
ModuleTableMap::ModuleTableMap() : m_maps({}), m_module(nullptr) {}

ModuleTableEntryPtr ModuleTableMap::get(const std::vector<std::string> &name) {
  return get(name.cbegin(), name.cend());
}
ModuleTableEntryPtr
ModuleTableMap::get(std::vector<std::string>::const_iterator curr,
                    std::vector<std::string>::const_iterator end) {
  if (curr == end) {
    return m_module;
  }

  auto nextMap = m_maps[*curr];
  if (nullptr == nextMap) {
    return nullptr;
  }
  return nextMap->get(curr + 1, end);
}

void ModuleTableMap::insert(const std::vector<std::string> &name,
                            ModuleTableEntryPtr module) {
  insert(name.cbegin(), name.cend(), module);
}
void ModuleTableMap::insert(std::vector<std::string>::const_iterator curr,
                            std::vector<std::string>::const_iterator end,
                            ModuleTableEntryPtr module) {
  if (curr == end) {
    m_module = module;
    return;
  }

  auto nextMap = m_maps[*curr];
  if (nullptr == nextMap) {
    nextMap = std::make_shared<ModuleTableMap>();
    m_maps[*curr] = nextMap;
  }
  return nextMap->insert(curr + 1, end, module);
}

std::string ModuleTableMap::toString() const { return "ModuleTableMap"; }

ModuleTableEntry::ModuleTableEntry(const std::vector<std::string> &name,
                                   std::shared_ptr<llvm::Module> module,
                                   std::shared_ptr<llvm::IRBuilder<>> builder)
    : m_name(name), m_module(module), m_builder(builder), m_functions({}) {}

ModuleTableEntryPtr
ModuleTableEntry::make(const std::vector<std::string> &name,
                       std::shared_ptr<llvm::LLVMContext> context) {
  std::string moduleName = Formatter<>::joinContainer('.', name);
  auto module = std::make_shared<llvm::Module>(std::move(moduleName), *context);
  auto builder = std::make_shared<llvm::IRBuilder<>>(*context);
  return std::shared_ptr<ModuleTableEntry>(
      new ModuleTableEntry(name, module, builder));
}

std::string ModuleTableEntry::toString() const {
  std::string str;
  llvm::raw_string_ostream rso{str};
  rso << *m_module;
  rso.flush();
  return str;
}
}; // namespace rgl
