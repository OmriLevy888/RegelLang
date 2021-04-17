#pragma once
#include <vector>

#include "parser/ast/constructs/Type.hpp"

namespace rgl {
class BasicType : public Type {
public:
  static TypePtr
  make(std::vector<std::string> &&name,
       BitField<TypeProperties> properties = TypeProperties::_default);
  static TypePtr
  make(const std::vector<std::string> &name,
       BitField<TypeProperties> properties = TypeProperties::_default);

  virtual bool isSimpleType() const noexcept override;

  virtual size_t getHash() const override;

  virtual TypePtr getOwningType() const override;
  virtual TypePtr getMutableType() const override;
  virtual TypePtr getValueType() const override;
  virtual TypePtr getUniquePointerType() const override;
  virtual TypePtr getSharedPointerType() const override;

  virtual std::string toTreeStr(size_t spaces) const override;

  static TypePtr t_implicit();
  static TypePtr t_void();
  static TypePtr t_int8();
  static TypePtr t_int16();
  static TypePtr t_int32();
  static TypePtr t_int64();
  static TypePtr t_uint8();
  static TypePtr t_uint16();
  static TypePtr t_uint32();
  static TypePtr t_uint64();
  static TypePtr t_float();
  static TypePtr t_double();
  static TypePtr t_char();
  static TypePtr t_string();
  static TypePtr t_bool();

private:
  std::vector<std::string> m_name;

  BasicType(std::vector<std::string> &&name,
            BitField<TypeProperties> properties = TypeProperties::_default)
      : Type(properties), m_name(std::move(name)) {}
  BasicType(const std::vector<std::string> &name,
            BitField<TypeProperties> properties = TypeProperties::_default)
      : Type(properties), m_name(name) {}
  BasicType(const std::string &name,
            BitField<TypeProperties> properties = TypeProperties::_default)
      : Type(properties), m_name({name}) {}
};
}; // namespace rgl
