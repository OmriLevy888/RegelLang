#pragma once
#include "common/Core.hpp"
#include "common/ILoggable.hpp"

namespace rgl {
class Type : public ILoggable {
public:
  std::string toString() const { return ""; }
};
}; // namespace rgl
