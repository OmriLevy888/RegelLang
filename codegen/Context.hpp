#pragma once
#include "codegen/CompilationContext.hpp"
#include "codegen/ModuleTable.hpp"
#include <thread>

namespace rgl {
class Context {
public:
  static void setCurrContext(const std::vector<std::string> &name) {
    ModuleTableEntryPtr module = nullptr;
    if (0 == name.size()) {
      module = get().m_modules->getMainEntry();
    } else {
      module = get().m_modules->getEntry(name);
      if (nullptr == module) {
        get().m_modules->createModule(name);
        module = get().m_modules->getEntry(name);
      }
    }

    size_t threadID = std::hash<std::thread::id>()(std::this_thread::get_id());
    std::shared_ptr currCompilationContext =
        get().m_compilationContexts[threadID];

    if (nullptr == currCompilationContext ||
        module != currCompilationContext->module()) {
      get().m_compilationContexts[threadID] =
          std::make_shared<CompilationContext>(get().m_llvmContext, module);
    }
  }

  static std::shared_ptr<CompilationContext> getCurrContext() {
    return get().m_compilationContexts[std::hash<std::thread::id>()(
        std::this_thread::get_id())];
  }
  static std::shared_ptr<ModuleTableEntry> module() {
    return getCurrContext()->module();
  }
  static llvm::Module *llvmModule() { return getCurrContext()->llvmModule(); }
  static llvm::IRBuilder<> *builder() { return getCurrContext()->builder(); }

  static std::shared_ptr<ModuleTable> modules() { return get().m_modules; }
  static llvm::LLVMContext *llvmContext() { return get().m_llvmContext.get(); }

private:
  std::unordered_map<size_t, std::shared_ptr<CompilationContext>>
      m_compilationContexts;
  std::shared_ptr<llvm::LLVMContext> m_llvmContext;
  std::shared_ptr<ModuleTable> m_modules;

  Context() : m_llvmContext(std::make_shared<llvm::LLVMContext>()) {
    m_modules = std::make_shared<ModuleTable>(m_llvmContext);
  }

  static Context &get() {
    static Context instance{};
    static bool initialized = false;
    if (!initialized) {
      initialized = true;
      setCurrContext(std::vector<std::string>{});
    }
    return instance;
  }
};
}; // namespace rgl
