#pragma once
#include "codegen/values/ValueBase.hpp"
#include "common/ILoggable.hpp"
#include <memory>

namespace rgl {
class SymbolBase : public ValueBase {
public:
  // Clean when cleaning a namespace (SymbolMap)
  virtual void clean(){};

protected:
  SymbolBase(llvm::Value *llvmValue, bool success = true)
      : ValueBase(llvmValue, success) {}
  SymbolBase(llvm::Module *llvmModule, bool success = true)
      : ValueBase(llvmModule, success) {}
  SymbolBase(llvm::Type *llvmType, bool success = true)
      : ValueBase(llvmType, success) {}
};

using SymbolPtr = std::shared_ptr<SymbolBase>;
}; // namespace rgl
