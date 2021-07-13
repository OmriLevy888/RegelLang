#pragma once
#include "parser/ast/constructs/TypeNodeBase.hpp"

namespace rgl {
class FunctionTypeNode;
using FunctionTypeNodePtr = std::unique_ptr<FunctionTypeNode>;

class FunctionTypeNode : public TypeNodeBase {
public:
  FunctionTypeNode(std::vector<TypeNodePtr> &&params, TypeNodePtr retType,
                   BitField<TypeProperties> properties)
      : TypeNodeBase(properties | TypeProperties::_isFunction),
        m_params(std::move(params)), m_retType(std::move(retType)) {}

  virtual bool isFunctionType() const noexcept override { return true; }

  virtual TypeNodePtr getOwningType() const override;
  virtual TypeNodePtr getMutableType() const override;
  virtual TypeNodePtr getValueType() const override;
  virtual TypeNodePtr getUniquePointerType() const override;
  virtual TypeNodePtr getSharedPointerType() const override;

  virtual TypeNodePtr clone() const override;

  virtual std::string toTreeStr(size_t spaces) const override;

  std::vector<TypeNodePtr> &paramTypes() { return m_params; }
  TypeNodePtr &retType() { return m_retType; }

private:
  std::vector<TypeNodePtr> m_params;
  TypeNodePtr m_retType;
};
}; // namespace rgl
