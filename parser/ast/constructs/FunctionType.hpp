#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "parser/ast/constructs/Type.hpp"

namespace rgl {
class FunctionType : public Type {
public:
  static TypePtr
  make(std::vector<TypePtr> &&params, TypePtr retType = nullptr,
       BitField<TypeProperties> properties = TypeProperties::_isFunction);

  static TypePtr
  make(const std::vector<TypePtr> &params, TypePtr retType = nullptr,
       BitField<TypeProperties> properties = TypeProperties::_isFunction);

  virtual size_t getHash() const override;

  virtual TypePtr getOwningType() const override;
  virtual TypePtr getMutableType() const override;
  virtual TypePtr getValueType() const override;
  virtual TypePtr getUniquePointerType() const override;
  virtual TypePtr getSharedPointerType() const override;

  virtual std::string toTreeStr(size_t spaces) const override;

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
}; // namespace rgl
