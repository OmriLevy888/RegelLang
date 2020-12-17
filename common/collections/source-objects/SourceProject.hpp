#pragma once

#include "common/Core.hpp"
#include "common/collections/source-objects/SourceFile.hpp"

namespace rgl {
class SourceProject : public ILoggable {
public:
  std::string m_name;
  std::vector<SourceFile> m_files;

  SourceProject(const std::string &name) : m_name(name) {}

  // TODO: implement these before switching to multiple lexers/parsers
  void lock() {}
  void release() {}

  std::string toString() const override {
    return Formatter("SourceProject<name: \"{}\", files: {}>", m_name,
                     m_files.size())
        .toString();
  }
};
} // namespace rgl
