#pragma once
#include "common/Formatter.hpp"
#include "rapidjson/document.h"
#include <fstream>
#include <iterator>

namespace rgl {
class ProjectFileParser {
public:
  static bool parseProjectFile(const std::string &path,
                               const std::string &chosenTarget);

private:
  static void interpretFields();
  static void interpretField(std::string &fieldValue);
};
}; // namespace rgl
