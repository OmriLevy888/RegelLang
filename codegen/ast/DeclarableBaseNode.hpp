#pragma once
#include <memory>

namespace rgl {
class DeclarableBaseNode {
public:
  virtual void declare() = 0;
  virtual void define() = 0;
};

using DeclarablePtr = std::unique_ptr<DeclarableBaseNode>;
}; // namespace rgl
