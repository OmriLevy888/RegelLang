#pragma once

#include "common/ILoggable.hpp"
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
  size_t addFile(SourceFile &&file) {
    m_files.push_back(std::move(file));
    return m_files.size() - 1;
  }

  std::string toString() const override {
    return Formatter("SourceProject<name: \"{}\", files: {}>", m_name,
                     m_files.size())
        .toString();
  }
};
} // namespace rgl
