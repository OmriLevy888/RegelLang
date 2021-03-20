#pragma once
#include <type_traits>

namespace rgl {
template <typename TEnum> class BitField {
public:
  using TUnderlying = typename std::underlying_type<TEnum>::type;

  BitField() {}
  BitField(const TEnum &val) : m_value(static_cast<TUnderlying>(val)) {}
  BitField(TEnum &&val) : m_value(static_cast<TUnderlying>(val)) {}
  BitField(const TUnderlying &val) : m_value(val) {}
  BitField(TUnderlying &&val) : m_value(val) {}
  BitField(const BitField<TEnum> &other) : m_value(other.m_value) {}
  BitField(BitField<TEnum> &&other) : m_value(other.m_value) {}

  BitField<TEnum> &operator=(const BitField<TEnum> &other) {
    m_value = other.m_value;
    return *this;
  }

  operator TUnderlying() const { return m_value; }
  operator TEnum() const { return static_cast<TEnum>(m_value); }

  BitField<TEnum> operator|(const BitField<TEnum> &other) const {
    return BitField<TEnum>(m_value | other.m_value);
  }
  BitField<TEnum> &operator|=(const BitField<TEnum> &other) {
    m_value = m_value | other.m_value;
    return *this;
  }

  BitField<TEnum> operator&(const BitField<TEnum> &other) const {
    return BitField<TEnum>(m_value & other.m_value);
  }
  BitField<TEnum> &operator&=(const BitField<TEnum> &other) {
    m_value = m_value & other.m_value;
    return *this;
  }

  BitField<TEnum> operator^(const BitField<TEnum> &other) const {
    return BitField<TEnum>(m_value ^ other.m_value);
  }
  BitField<TEnum> &operator^=(const BitField<TEnum> &other) {
    m_value = m_value ^ other.m_value;
    return *this;
  }

  BitField<TEnum> operator~() const { return BitField<TEnum>(~m_value); }

  bool operator[](const TUnderlying &bit) const {
    TUnderlying mask = static_cast<TUnderlying>(-1) ^ bit;
    return (m_value & mask) != m_value;
  }

  bool operator==(const BitField<TEnum> &other) const {
    return m_value = other.m_value;
  }
  bool operator!=(const BitField<TEnum> &other) const {
    return m_value != other.m_value;
  }

private:
  TUnderlying m_value;
};
}; // namespace rgl
