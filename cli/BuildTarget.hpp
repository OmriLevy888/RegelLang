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

  static std::optional<BuildTarget> makeTarget(const rapidjson::Value &target) {
    if (!target.IsObject()) {
      return std::nullopt;
    }

    BuildTarget ret;
    if (target.HasMember("Output")) {
      if (target["Output"].IsString()) {
        ret.m_output = target["Output"].GetString();
      } else {
        // TODO: write warning message
      }
    }

    if (target.HasMember("Files")) {
      if (target["Files"].IsString()) {
        ret.m_files = std::vector<std::string>();
        ret.m_files.push_back(target["Files"].GetString());
      } else if (target["Files"].IsArray()) {
        ret.m_files = std::vector<std::string>();
        const auto &files = target["Files"].GetArray();
        for (rapidjson::SizeType idx = 0; idx < files.Size(); idx++) {
          const auto &curr = files[idx];
          if (!curr.IsString()) {
            // TODO: write warning message
            continue;
          }
          const std::string &file = curr.GetString();
          if (file[0] == '/')
            ret.m_files.push_back(file);
          else { // rebase from the project directory
            ret.m_files.push_back(
                Formatter<>::join('/', "${ProjectDir}", file));
          }
        }
      } else {
        // TODO: write warning message
        return std::nullopt;
      }
    }

    return ret;
  }

  std::string toString() const override {
    return Formatter("BuildTarget<output: {}>", m_output).toString();
  }
};
}; // namespace rgl
