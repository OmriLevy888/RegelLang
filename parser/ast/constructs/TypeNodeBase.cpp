#include <functional>
#include <unordered_map>

#include "parser/ast/constructs/TypeNodeBase.hpp"

namespace rgl {
TypeProperties operator~(TypeProperties property) {
  using TUnderlying = typename std::underlying_type<TypeProperties>::type;
  return static_cast<TypeProperties>(~static_cast<TUnderlying>(property));
}

std::string typePropertiesToString(BitField<TypeProperties> properties) {
  return (properties & TypeProperties::_owning)    ? (":")
         : (properties & TypeProperties::_mutable) ? ("&")
                                                   : ("");
}

bool TypeNodeBase::isMutable() const noexcept {
  return m_typeProperties & TypeProperties::_mutable;
}
bool TypeNodeBase::isConst() const noexcept { return !this->isMutable(); }
bool TypeNodeBase::isOwning() const noexcept {
  return m_typeProperties & TypeProperties::_owning;
}
bool TypeNodeBase::isPointer() const noexcept {
  return this->isUniquePointer() || this->isSharedPointer();
}
bool TypeNodeBase::isUniquePointer() const noexcept {
  return m_typeProperties & TypeProperties::_isPointer;
}
bool TypeNodeBase::isSharedPointer() const noexcept {
  return m_typeProperties & TypeProperties::_isShared;
}
}; // namespace rgl
