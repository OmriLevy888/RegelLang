#include "codegen/ModuleTableEntry.hpp"
#include "llvm/Support/raw_ostream.h"

namespace rgl {
ModuleTableEntry::ModuleTableEntry(const std::string &name,
                                   std::shared_ptr<llvm::LLVMContext> context)
    : m_module(std::make_shared<llvm::Module>(name, *context)),
      m_builder(std::make_shared<llvm::IRBuilder<>>(*context)) {}

std::string ModuleTableEntry::toString() const {
  std::string str;
  llvm::raw_string_ostream rso{str};
  rso << *m_module;
  rso.flush();
  return str;
}
}; // namespace rgl
