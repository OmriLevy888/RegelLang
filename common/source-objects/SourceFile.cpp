#include "common/Formatter.hpp"
#include "common/source-objects/SourceFile.hpp"

namespace rgl {
std::string SourceFile::toString() const {
  return Formatter("SourceFile<path: \"{}\", lines: {}>", m_path,
                   m_lines.size())
      .toString();
}
} // namespace rgl
