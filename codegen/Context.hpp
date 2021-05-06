#pragma once
#include "codegen/CompilationContext.hpp"
#include "codegen/ModuleTable.hpp"

namespace rgl {
class Context {
public:
  static std::shared_ptr<CompilationContext> getCurrContext() {
    return get().m_compilationContext;
  }

  static std::shared_ptr<ModuleTable> modules() { return get().m_modules; }
  static llvm::LLVMContext *llvmContext() { return get().m_llvmContext.get(); }

private:
  std::shared_ptr<CompilationContext> m_compilationContext;
  std::shared_ptr<llvm::LLVMContext> m_llvmContext;
  std::shared_ptr<ModuleTable> m_modules;

  Context() : m_llvmContext(std::make_shared<llvm::LLVMContext>()) {
    m_modules = std::make_shared<ModuleTable>(m_llvmContext);

    m_compilationContext = std::make_shared<CompilationContext>(
        m_llvmContext, m_modules->getMainEntry());
  }

  static Context &get() {
    static Context instance{};
    return instance;
  }
};
}; // namespace rgl
