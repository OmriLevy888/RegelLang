#pragma once
#include "common/Core.h"
#include "ISourceStream.h"

namespace rgl {
class TextSourceStream : public ISourceStream {
public:
    TextSourceStream(const std::string& source) 
    : m_sourceText(source), m_pos(0), m_end(source.length())
    {}
    TextSourceStream(std::string&& source) 
    : m_sourceText(source), m_pos(0), m_end(source.length())
    {}

    int peek() override;
    int read() override;
    bool readLine(std::string &line) override;
    size_t tell() const override;
    size_t seek(off_t offset, std::ios_base::seekdir whence = std::ios_base::beg) override;
    inline bool eof() const override {
        return m_pos > m_end;
    }
    inline bool isOpen() const override {
        return m_pos < m_end;
    }

    std::string toString() const override;

private:
    const std::string m_sourceText;
    size_t m_pos;
    size_t m_end;
};
}  // namespace rgl