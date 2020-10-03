#pragma once
#include "common/Core.h"
#include "lexer/Token.h"

namespace rgl {
class ITokenGenerator : public ILoggable {
public:
    virtual Token getNext() = 0;
};
} // namespace rgl