#include "common/Formatter.hpp"
#include "common/source-objects/SourceProject.hpp"

namespace rgl {
std::string SourceProject::toString() const {
  return Formatter("SourceProject<name: \"{}\", files: {}>", m_name,
                   m_files.size())
      .toString();
}
} // namespace rgl
