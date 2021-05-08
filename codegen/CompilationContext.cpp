#include "codegen/CompilationContext.hpp"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

namespace rgl {
CompilationContext::CompilationContext(
    std::shared_ptr<llvm::LLVMContext> context,
    std::shared_ptr<ModuleTableEntry> module)
    : m_context(context), m_module(module) {}

std::string CompilationContext::toString() const {
  return "CompilationContext";
}
}; // namespace rgl
