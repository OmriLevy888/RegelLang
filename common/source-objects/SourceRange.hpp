#pragma once
#include "common/ILoggable.hpp"
#include "common/source-objects/SourceLocation.hpp"

namespace rgl {
class SourceRange : public ILoggable {
public:
  uint32_t m_file;
  uint32_t m_startLine;
  uint32_t m_startOffset;
  uint32_t m_endLine;
  uint32_t m_endOffset;

  SourceRange(uint32_t file = 0, uint32_t startLine = 0, uint32_t startOffset = 0,
              uint32_t endLine = 0, uint32_t endOffset = 0)
    : m_file(file), m_startLine(startLine), m_startOffset(startOffset),
      m_endLine(endLine), m_endOffset(endOffset) {}
  SourceRange(const SourceLocation &location);

  SourceRange to(const SourceRange &other) const;

  bool isEmptyRange() const;
  
  virtual std::string toString() const override;
};  
} // namespace rgl
