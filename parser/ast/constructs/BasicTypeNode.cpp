#include "parser/ast/constructs/BasicTypeNode.hpp"

namespace rgl {
BasicTypeNode::BasicTypeNode(std::vector<std::string> &&name,
                             BitField<TypeProperties> properties)
    : BasicTypeNode(std::make_unique<IdentifierNode>(std::move(name)),
                    properties) {}

bool BasicTypeNode::isImplicitType() const noexcept {
  return 1 == m_name->size() && m_name->get()[0] == "@implicit";
}
bool BasicTypeNode::isSimpleType() const noexcept {
  return (1 == m_name->size()) &&
         (m_typeProperties ==
          BitField<TypeProperties>(TypeProperties::_default));
}

TypeNodePtr BasicTypeNode::getOwningType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() | TypeProperties::_owning;
  return ret;
}
TypeNodePtr BasicTypeNode::getMutableType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() | TypeProperties::_mutable;
  return ret;
}
TypeNodePtr BasicTypeNode::getValueType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() & ~TypeProperties::_isPointer &
                          ~TypeProperties::_isShared;
  return ret;
}
// TODO: fix these casts
TypeNodePtr BasicTypeNode::getUniquePointerType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() | TypeProperties::_isPointer;
  return ret;
}
TypeNodePtr BasicTypeNode::getSharedPointerType() const {
  auto ret = this->clone();
  ret->typeProperties() = ret->typeProperties() | TypeProperties::_isPointer |
                          TypeProperties::_isShared;
  return ret;
}

std::string BasicTypeNode::toTreeStr(size_t spaces) const {
  std::string typePrefix = (m_typeProperties & TypeProperties::_owning) ? (":")
                           : (m_typeProperties & TypeProperties::_mutable)
                               ? ("&")
                               : ("");

  if (m_typeProperties & TypeProperties::_isPointer) {
    typePrefix +=
        (m_typeProperties & TypeProperties::_isShared) ? ("{}") : ("<>");
  }

  return Formatter("Type<{}{}>", typePrefix, m_name->getString());
}

TypeNodePtr BasicTypeNode::clone() const {
  return std::make_unique<BasicTypeNode>(m_name->clone(), m_typeProperties);
}

TypeNodePtr BasicTypeNode::t_implicit() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"@implicit"});
}
TypeNodePtr BasicTypeNode::t_void() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"void"});
}
TypeNodePtr BasicTypeNode::t_int8() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"i8"});
}
TypeNodePtr BasicTypeNode::t_int16() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"i16"});
}
TypeNodePtr BasicTypeNode::t_int32() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"i32"});
}
TypeNodePtr BasicTypeNode::t_int64() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"i64"});
}
TypeNodePtr BasicTypeNode::t_uint8() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"u8"});
}
TypeNodePtr BasicTypeNode::t_uint16() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"u16"});
}
TypeNodePtr BasicTypeNode::t_uint32() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"u32"});
}
TypeNodePtr BasicTypeNode::t_uint64() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"u64"});
}
TypeNodePtr BasicTypeNode::t_float() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"float"});
}
TypeNodePtr BasicTypeNode::t_double() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"double"});
}
TypeNodePtr BasicTypeNode::t_char() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"char"});
}
TypeNodePtr BasicTypeNode::t_string() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"string"});
}
TypeNodePtr BasicTypeNode::t_bool() {
  return std::make_unique<BasicTypeNode>(std::vector<std::string>{"bool"});
}
}; // namespace rgl
