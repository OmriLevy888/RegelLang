#pragma once
#include "common/ILoggable.hpp"
#include "common/source-objects/SourceFile.hpp"

namespace rgl {
class SourceProject : public ILoggable {
public:
  static SourceProject &get() {
    static SourceProject instance;
    return instance;
  }

  static void clean() {
    get().m_name = "";
    get().m_files.clear();
  }

  std::string &name() { return m_name; }
  std::string name() const { return m_name; }
  std::vector<SourceFile> &files() { return m_files; }
  std::vector<SourceFile> files() const { return m_files; }

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

private:
  std::string m_name;
  std::vector<SourceFile> m_files;

  SourceProject() {}
};
} // namespace rgl
