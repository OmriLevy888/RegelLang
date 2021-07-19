#pragma once
#include "common/ILoggable.hpp"

namespace rgl {
class SourceLocation : public ILoggable {
public:
  uint32_t m_file;
  uint32_t m_line;
  uint32_t m_reprStartIdx;
  uint32_t m_reprLen;

  SourceLocation(uint32_t file = 0, uint32_t line = 0,
                 uint32_t reprStartIdx = 0, uint32_t reprLen = 0);

  virtual std::string toString() const override;

private:
};
}; // namespace rgl
