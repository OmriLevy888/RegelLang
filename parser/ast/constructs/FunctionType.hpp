#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "parser/ast/constructs/Type.hpp"
#include "llvm/IR/DerivedTypes.h"

namespace rgl {
class FunctionType : public Type {
public:
  static std::shared_ptr<FunctionType>
  make(std::vector<TypePtr> &&params, TypePtr retType = nullptr,
       BitField<TypeProperties> properties = TypeProperties::_isFunction);

  static std::shared_ptr<FunctionType>
  make(const std::vector<TypePtr> &params, TypePtr retType = nullptr,
       BitField<TypeProperties> properties = TypeProperties::_isFunction);

  virtual size_t getHash() const override;

  virtual TypePtr getOwningType() const override;
  virtual TypePtr getMutableType() const override;
  virtual TypePtr getValueType() const override;
  virtual TypePtr getUniquePointerType() const override;
  virtual TypePtr getSharedPointerType() const override;

  virtual std::string toTreeStr(size_t spaces) const override;

  virtual llvm::FunctionType *toLLVMType() override;

  std::vector<TypePtr> paramTypes() { return m_params; }
  TypePtr retType() { return m_retType; }

private:
  std::vector<TypePtr> m_params;
  TypePtr m_retType;

  FunctionType(std::vector<TypePtr> &&params, TypePtr retType,
               BitField<TypeProperties> properties)
      : Type(properties | TypeProperties::_isFunction,
             Type::getPointerSizeBytes()),
        m_params(std::move(params)), m_retType(retType) {}

  FunctionType(const std::vector<TypePtr> &params, TypePtr retType,
               BitField<TypeProperties> properties)
      : Type(properties | TypeProperties::_isFunction,
             Type::getPointerSizeBytes()),
        m_params(params), m_retType(retType) {}
};

using FunctionTypePtr = std::shared_ptr<FunctionType>;
}; // namespace rgl
