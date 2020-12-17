#pragma once
#include "ISourceStream.hpp"
#include "common/Core.hpp"

namespace rgl {
class FileSourceStream : public ISourceStream {
public:
  FileSourceStream(const std::string &filePath, size_t fileIndex)
      : m_filePath(filePath), m_fileIndex(fileIndex),
        m_ifstream(filePath, std::ifstream::binary) {}

  int peek() override;
  int read() override;
  bool readLine(std::string &line) override;
  size_t tell() const override;
  size_t seek(off_t offset,
              std::ios_base::seekdir whence = std::ios_base::beg) override;
  inline bool eof() const override { return m_ifstream.eof() || !isOpen(); }
  inline bool isOpen() const override { return static_cast<bool>(m_ifstream); }
  size_t getFileIndex() const override { return m_fileIndex; }

  std::string toString() const override;

private:
  const std::string m_filePath;
  const size_t m_fileIndex;
  mutable std::ifstream m_ifstream;
};
} // namespace rgl
