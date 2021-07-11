#pragma once
#include "codegen/values/symbols/SymbolBase.hpp"
#include "common/utils/BitField.hpp"

namespace rgl {
class TypeSymbolBase;
using TypeSymbolPtr = std::shared_ptr<TypeSymbolBase>;

enum class TypeTraits : uint64_t { TriviallyCopiable };

class TypeSymbolBase : public SymbolBase {
public:
  virtual bool isType() const override { return true; }
  virtual bool isBasicType() const { return false; }
  virtual bool isFunctionType() const { return false; }
  virtual bool isArrayType() const { return false; }
  virtual bool isModifiedType() const { return false; }

protected:
  BitField<TypeTraits> m_traits;

  TypeSymbolBase(const std::vector<std::string> &name, llvm::Type *llvmType);
};
}; // namespace rgl
