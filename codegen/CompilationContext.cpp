#include "codegen/CompilationContext.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/symbols/types/BasicTypeSymbol.hpp"
#include "llvm/IR/Type.h"

namespace rgl {
CompilationContext::CompilationContext(
    std::shared_ptr<llvm::LLVMContext> context, ModuleSymbolPtr module)
    : m_context(context), m_module(module),
      m_generatingFunctions(std::stack<FunctionSymbolPtr>{}) {
  initializeBuiltinTypes();
}

void CompilationContext::initializeBuiltinTypes() {
  struct builtinTypeDesc {
    llvm::Type *llvmType;
    std::vector<std::string> typeNames;
  };
  std::vector<builtinTypeDesc> builtinTypes{
      {llvm::Type::getVoidTy(*Context::llvmContext()), {"void"}},
      {llvm::Type::getInt1Ty(*Context::llvmContext()), {"bool"}},
      {llvm::Type::getInt8Ty(*Context::llvmContext()), {"i8", "u8", "char"}},
      {llvm::Type::getInt16Ty(*Context::llvmContext()), {"i16", "u16"}},
      {llvm::Type::getInt32Ty(*Context::llvmContext()), {"i32", "u32"}},
      {llvm::Type::getInt64Ty(*Context::llvmContext()), {"i64", "u64"}},
      {llvm::Type::getFloatTy(*Context::llvmContext()), {"float"}},
      {llvm::Type::getDoubleTy(*Context::llvmContext()), {"double"}}};

  const BitField<TypeTraits> defaultTraits = TypeTraits::TriviallyCopiable;
  for (const auto &type : builtinTypes) {
    for (const auto &name : type.typeNames) {
      auto typeSymbol =
          BasicTypeSymbol::make(type.llvmType, defaultTraits, name);
      std::vector<std::string> nameVector;
      nameVector.push_back(name);
      // use m_module rather than Context::module() to make sure there are no
      // recursive calls to CompilationContext::initializeBuiltinTypes
      m_module->symbols().insert(nameVector, typeSymbol);
    }
  }
}

std::string CompilationContext::toString() const {
  return "CompilationContext";
}
}; // namespace rgl
