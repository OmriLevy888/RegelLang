#pragma once
#include "codegen/values/ValueBase.hpp"

namespace rgl {
class BasicValue : public ValueBase {
public:
  explicit BasicValue(llvm::Value *llvmValue, bool success = true)
      : ValueBase(llvmValue, success) {}
};

using BasicValuePtr = std::shared_ptr<BasicValue>;
}; // namespace rgl
