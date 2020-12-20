#pragma once
#include <string>

namespace rgl {
class ILoggable {
public:
  virtual std::string toString() const = 0;

  operator std::string() const { return this->toString(); }
};
} // namespace rgl
