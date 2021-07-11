#pragma once
#include "codegen/CompilationContext.hpp"
#include "codegen/values/symbols/SymbolMap.hpp"
#include <thread>

namespace rgl {
const std::vector<std::string> MAIN_MODULE_NAME{"main"};

class Context {
public:
  static void
  setCurrContext(const std::vector<std::string> &name = MAIN_MODULE_NAME) {
    SymbolPtr symbol = get().m_modules.get(name);
    if (nullptr == symbol) {
    }
    ModuleSymbolPtr module = std::dynamic_pointer_cast<ModuleSymbol>(symbol);
    if (nullptr == module) {
      module = ModuleSymbol::make(name, get().m_llvmContext);
      get().m_modules.insert(name, module);
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
  static ModuleSymbolPtr module() { return getCurrContext()->module(); }
  static llvm::Module *llvmModule() { return getCurrContext()->llvmModule(); }
  static llvm::IRBuilder<> *builder() { return getCurrContext()->builder(); }

  static llvm::LLVMContext *llvmContext() { return get().m_llvmContext.get(); }

private:
  std::unordered_map<size_t, std::shared_ptr<CompilationContext>>
      m_compilationContexts;
  std::shared_ptr<llvm::LLVMContext> m_llvmContext;
  SymbolMap m_modules;

  Context()
      : m_compilationContexts({}),
        m_llvmContext(std::make_shared<llvm::LLVMContext>()), m_modules({}) {}

  static Context &get() {
    static Context instance{};
    static bool initialized = false;
    if (!initialized) {
      initialized = true;
      setCurrContext();
    }
    return instance;
  }
};
}; // namespace rgl
