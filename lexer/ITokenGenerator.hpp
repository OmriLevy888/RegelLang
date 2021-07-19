#pragma once
#include "common/ILoggable.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <variant>

namespace rgl {
class ITokenGenerator : public ILoggable {
public:
  virtual TokenValuePair getNext() = 0;
};
}; // namespace rgl
