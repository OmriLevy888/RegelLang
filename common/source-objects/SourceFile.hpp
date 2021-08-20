#pragma once
#include "common/ILoggable.hpp"
#include "common/source-objects/SourceLine.hpp"

namespace rgl {
class SourceFile : public ILoggable {
public:
  std::string m_path;
  std::vector<SourceLine> m_lines;

  SourceFile(const std::string &path) : m_path(path) {}

  std::string toString() const override;
};
} // namespace rgl
