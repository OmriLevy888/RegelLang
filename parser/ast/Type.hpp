#pragma once
#include "common/Core.hpp"
#include "common/ILoggable.hpp"

namespace rgl {
class Type : public ILoggable {
public:
  Type() : m_name(t_void()->m_name), m_isReference(t_void()->m_isReference) {}

  Type(const std::string &name, bool isReference = false)
      : m_name(name), m_isReference(isReference) {}

  std::string toString() const override {
    std::string referencePrefix = (m_isReference) ? ("&") : ("");
    return Formatter("Type<{}{}>", referencePrefix, m_name);
  }

private:
  std::string m_name; // change this in the future for some other type that
                      // better represents the name of the type
  bool m_isReference;

public:
  static std::shared_ptr<Type> t_void() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("void");
    return instance;
  }
  static std::shared_ptr<Type> t_int8() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("int8");
    return instance;
  }
  static std::shared_ptr<Type> t_int16() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("int16");
    return instance;
  }
  static std::shared_ptr<Type> t_int32() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("int32");
    return instance;
  }
  static std::shared_ptr<Type> t_int64() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("int64");
    return instance;
  }
  static std::shared_ptr<Type> t_uint8() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("uint8");
    return instance;
  }
  static std::shared_ptr<Type> t_uint16() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("uint16");
    return instance;
  }
  static std::shared_ptr<Type> t_uint32() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("uint32");
    return instance;
  }
  static std::shared_ptr<Type> t_uint64() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("uint64");
    return instance;
  }
  static std::shared_ptr<Type> t_float() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("float");
    return instance;
  }
  static std::shared_ptr<Type> t_double() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("double");
    return instance;
  }
  static std::shared_ptr<Type> t_char() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("char");
    return instance;
  }
  static std::shared_ptr<Type> t_string() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("string");
    return instance;
  }
  static std::shared_ptr<Type> t_bool() {
    static std::shared_ptr<Type> instance = std::make_shared<Type>("bool");
    return instance;
  }
};
}; // namespace rgl
