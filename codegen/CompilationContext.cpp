#include "codegen/CompilationContext.hpp"

namespace rgl {
CompilationContext::CompilationContext(
    std::shared_ptr<llvm::LLVMContext> context, ModuleSymbolPtr module)
    : m_context(context), m_module(module) {}

std::string CompilationContext::toString() const {
  return "CompilationContext";
}
}; // namespace rgl
