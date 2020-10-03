#pragma once
#include "Core.h"

namespace rgl {
class ILoggable {
public:
    virtual std::string toString() const = 0;
};
}  // namespace rgl