#include "codegen/CompilationContext.hpp"

namespace rgl {
CompilationContext::CompilationContext(
    std::shared_ptr<llvm::LLVMContext> context, ModuleSymbolPtr module)
    : m_context(context), m_module(module),
      m_generatingFunctions(std::stack<FunctionSymbolPtr>{}) {}

std::string CompilationContext::toString() const {
  return "CompilationContext";
}
}; // namespace rgl
