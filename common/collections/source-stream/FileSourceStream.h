#pragma once
#include "common/Core.h"
#include "ISourceStream.h"

namespace rgl {
class FileSourceStream : public ISourceStream {
public:
    FileSourceStream(const std::string& filePath) 
    : m_filePath(filePath), m_ifstream(filePath, std::ifstream::binary) 
    {}

    int peek() override;
    int read() override;
    bool readLine(std::string &line) override;
    size_t tell() const override;
    size_t seek(off_t offset, std::ios_base::seekdir whence = std::ios_base::beg) override;
    inline bool eof() const override {
        return m_ifstream.eof() || !isOpen();
    }
    inline bool isOpen() const override {
        return static_cast<bool>(m_ifstream);
    }

    std::string toString() const override;

private:
    const std::string m_filePath;
    mutable std::ifstream m_ifstream;
};
}  // namespace rgl