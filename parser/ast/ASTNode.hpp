#pragma once
#include "common/Core.hpp"
#include "common/ILoggable.hpp"

namespace rgl {
class ASTNode : public ILoggable {
public:
  virtual bool generateCode() = 0;

  void setLineNo(uint32_t val) noexcept {
    m_meta = ((val << 14) & 0xffffffff) | getFileNo();
  }
  void setFileNo(uint16_t val) noexcept {
    m_meta = getLineNo() | (val & 0x00003fff);
  }
  void setStartTokenIdx(uint16_t val) noexcept { m_startTokenIdx = val; }
  void setTokenCount(uint16_t val) noexcept { m_tokenCount = val; }

  uint32_t getLineNo() const noexcept { return m_meta & 0xffffc000; }
  uint16_t getFileNo() const noexcept { return m_meta & 0x00003fff; }
  uint16_t getStartTokenIdx() const noexcept { return m_startTokenIdx; }
  uint16_t getTokenCount() const noexcept { return m_tokenCount; }

private:
  uint32_t m_meta;
  uint16_t m_startTokenIdx;
  uint16_t m_tokenCount;
};
}; // namespace rgl
