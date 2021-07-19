#include "common/source-objects/SourceLocation.hpp"

namespace rgl {
SourceLocation::SourceLocation(uint32_t file, uint32_t line,
                               uint32_t reprStartIdx, uint32_t reprLen)
    : m_file(file), m_line(line), m_reprStartIdx(reprStartIdx),
      m_reprLen(reprLen) {}

std::string SourceLocation::toString() const { return "SourceLocation"; }
}; // namespace rgl
