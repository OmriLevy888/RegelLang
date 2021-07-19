#pragma once
#include <memory>

namespace rgl {
class ValueBase;
using ValuePtr = std::shared_ptr<ValueBase>;

class DeclarableNodeBase {
public:
  virtual void declare() = 0;
  virtual ValuePtr define() = 0;
};

using DeclarablePtr = std::unique_ptr<DeclarableNodeBase>;
}; // namespace rgl
