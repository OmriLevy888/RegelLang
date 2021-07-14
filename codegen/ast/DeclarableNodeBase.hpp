#pragma once
#include <memory>

namespace rgl {
class DeclarableNodeBase {
public:
  virtual void declare() = 0;
  virtual void define() = 0;
};

using DeclarablePtr = std::unique_ptr<DeclarableNodeBase>;
}; // namespace rgl
