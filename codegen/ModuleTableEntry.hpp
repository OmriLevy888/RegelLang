#pragma once
#include "codegen/FunctionTable.hpp"
#include "common/ILoggable.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <string>
#include <unordered_map>

namespace rgl {
class ModuleTableEntry;
using ModuleTableEntryPtr = std::shared_ptr<ModuleTableEntry>;

class ModuleTableMap : public ILoggable {
public:
  ModuleTableMap();

  ModuleTableEntryPtr get(const std::vector<std::string> &name);
  void insert(const std::vector<std::string> &name, ModuleTableEntryPtr);

  virtual std::string toString() const override;

private:
  std::unordered_map<std::string, std::shared_ptr<ModuleTableMap>> m_maps;
  ModuleTableEntryPtr m_module;

  ModuleTableEntryPtr get(std::vector<std::string>::const_iterator curr,
                          std::vector<std::string>::const_iterator end);
  void insert(std::vector<std::string>::const_iterator curr,
              std::vector<std::string>::const_iterator end,
              ModuleTableEntryPtr module);
};

class ModuleTableEntry : public ILoggable {
public:
  static ModuleTableEntryPtr make(const std::vector<std::string> &name,
                                  std::shared_ptr<llvm::LLVMContext> context);

  std::vector<std::string> getName() { return m_name; }

  llvm::Module *get() { return m_module.get(); }
  llvm::IRBuilder<> *builder() { return m_builder.get(); }
  FunctionTable &functions() { return m_functions; }

  virtual std::string toString() const override;

private:
  std::vector<std::string> m_name;
  std::shared_ptr<llvm::Module> m_module;
  std::shared_ptr<llvm::IRBuilder<>> m_builder;
  FunctionTable m_functions;

  ModuleTableEntry(const std::vector<std::string> &name,
                   std::shared_ptr<llvm::Module> module,
                   std::shared_ptr<llvm::IRBuilder<>> builder);
};
}; // namespace rgl
