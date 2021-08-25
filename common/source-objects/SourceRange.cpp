#include "common/source-objects/SourceRange.hpp"
#include "common/Formatter.hpp"

namespace rgl {
SourceRange::SourceRange(const SourceLocation &location)
  : m_file(location.m_file), m_startLine(location.m_line),
    m_startOffset(location.m_reprStartIdx), m_endLine(location.m_line),
    m_endOffset(location.m_reprStartIdx + location.m_reprLen) {}
  
SourceRange SourceRange::to(const SourceRange &other) const {
 return SourceRange{m_file, m_startLine, m_startOffset, other.m_endLine, other.m_endOffset};
}

bool SourceRange::isEmptyRange() const {
  return (m_startLine == m_endLine) && (m_startOffset == m_endOffset);
}
  
std::string SourceRange::toString() const {
  return Formatter("SourceRange<{}:{}:{}:{}:{}>", m_file, m_startLine,
                   m_startOffset, m_endLine, m_endOffset);
}
} // namespace rgl
