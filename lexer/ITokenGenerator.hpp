#pragma once
#include "common/Core.hpp"
#include "lexer/Token.hpp"

namespace rgl {
class ITokenGenerator : public ILoggable {
public:
  virtual Token getNext() = 0;
};
} // namespace rgl
