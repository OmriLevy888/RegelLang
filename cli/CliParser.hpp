#pragma once
#include "cli/CliArguments.hpp"

namespace rgl {
class CliParser {
public:
  static bool parseCliArgument(size_t argc, const char **argv);
};
}; // namespace rgl
