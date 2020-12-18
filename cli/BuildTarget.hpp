#pragma once
#include "common/Formatter.hpp"
#include "common/ILoggable.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include <optional>
#include <vector>

namespace rgl {
class BuildTarget : public ILoggable {
public:
  std::string m_output;
  std::vector<std::string> m_files;

  BuildTarget() : m_output("a.out") {}

  static std::optional<BuildTarget> makeTarget(const rapidjson::Value &target);

  std::string toString() const override {
    return Formatter("BuildTarget<output: {}>", m_output).toString();
  }
};
}; // namespace rgl
