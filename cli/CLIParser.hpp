#pragma once
#include "cli/CLIArguments.hpp"

namespace rgl {
class CLIParser {
public:
  static bool parseCliArgument(size_t argc, const char **argv);
};
}; // namespace rgl
