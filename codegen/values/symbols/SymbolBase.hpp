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
  SymbolBase(bool success) : ValueBase(success) {}
  SymbolBase(llvm::Value *llvmValue) : ValueBase(llvmValue) {}
  SymbolBase(llvm::Module *llvmModule) : ValueBase(llvmModule) {}
  SymbolBase(llvm::Type *llvmType) : ValueBase(llvmType) {}
};

using SymbolPtr = std::shared_ptr<SymbolBase>;
}; // namespace rgl
