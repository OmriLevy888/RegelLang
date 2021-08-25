#pragma once
#include "common/Core.hpp"
#include "common/ILoggable.hpp"
#include "common/source-objects/SourceRange.hpp"

namespace rgl {
class ASTNode : public ILoggable {
public:
  virtual std::string toString() const override { return toTreeStr(0); }
  virtual std::string toTreeStr(const size_t spaces) const = 0;
  
  virtual SourceRange getSourceRange() const { return SourceRange{}; }
};
}; // namespace rgl
