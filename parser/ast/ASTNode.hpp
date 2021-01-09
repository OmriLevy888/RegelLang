#pragma once
#include "common/Core.hpp"
#include "common/ILoggable.hpp"

namespace rgl {
class ASTNode : public ILoggable {
public:
  // virtual bool generateCode() = 0;

  virtual std::string toString() const override { return toTreeStr(0); }
  virtual std::string toTreeStr(size_t spaces) const = 0;
};
}; // namespace rgl
