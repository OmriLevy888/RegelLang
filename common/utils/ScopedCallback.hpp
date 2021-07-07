#pragma once

namespace rgl {
template <typename TCallback> class ScopedCallback {
public:
  ScopedCallback(TCallback callback, bool armed)
      : m_callback(callback), m_armed(armed) {}

  ~ScopedCallback() {
    if (m_armed) {
      m_callback();
    }
  }

  void arm() { m_armed = true; }
  void unarm() { m_armed = false; }
  void setArmed(bool value) { m_armed = value; }
  bool isArmed() { return m_armed; }

private:
  TCallback m_callback;
  bool m_armed;
};

template <typename TCallback>
class UnarmedScopedCallback : public ScopedCallback<TCallback> {
public:
  UnarmedScopedCallback(TCallback callback)
      : ScopedCallback<TCallback>(callback, false) {}
};

template <typename TCallback>
class ArmedScopedCallback : public ScopedCallback<TCallback> {
public:
  ArmedScopedCallback(TCallback callback)
      : ScopedCallback<TCallback>(callback, true) {}
};
}; // namespace rgl
