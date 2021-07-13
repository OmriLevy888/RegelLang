#pragma once
#include "parser/ast/constructs/TypeNodeBase.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class BasicTypeNode;
using BasicTypeNodePtr = std::unique_ptr<BasicTypeNode>;

class BasicTypeNode : public TypeNodeBase {
public:
  BasicTypeNode(std::vector<std::string> &&name,
                BitField<TypeProperties> properties = TypeProperties::_default);
  BasicTypeNode(Identifier name,
                BitField<TypeProperties> properties = TypeProperties::_default)
      : TypeNodeBase(properties), m_name(std::move(name)) {}

  virtual bool isBasicType() const noexcept override { return true; }
  virtual bool isImplicitType() const noexcept override;
  virtual bool isSimpleType() const noexcept override;

  virtual TypeNodePtr getOwningType() const override;
  virtual TypeNodePtr getMutableType() const override;
  virtual TypeNodePtr getValueType() const override;
  virtual TypeNodePtr getUniquePointerType() const override;
  virtual TypeNodePtr getSharedPointerType() const override;

  virtual TypeNodePtr clone() const override;

  virtual std::string toTreeStr(size_t spaces) const override;

  // mostly here to make life easier when writing tests
  static TypeNodePtr t_implicit();
  static TypeNodePtr t_void();
  static TypeNodePtr t_int8();
  static TypeNodePtr t_int16();
  static TypeNodePtr t_int32();
  static TypeNodePtr t_int64();
  static TypeNodePtr t_uint8();
  static TypeNodePtr t_uint16();
  static TypeNodePtr t_uint32();
  static TypeNodePtr t_uint64();
  static TypeNodePtr t_float();
  static TypeNodePtr t_double();
  static TypeNodePtr t_char();
  static TypeNodePtr t_string();
  static TypeNodePtr t_bool();

private:
  Identifier m_name;
};
}; // namespace rgl
