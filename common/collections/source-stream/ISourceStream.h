#pragma once
#include "common/Core.h"

namespace rgl {
class ISourceStream : public ILoggable {
public:
    virtual int peek() = 0;
    virtual int read() = 0;
    virtual bool readLine(std::string &line) = 0;
    virtual size_t tell() const = 0;
    virtual size_t seek(off_t offset, std::ios_base::seekdir = std::ios_base::beg) = 0;
    virtual bool eof() const = 0;
    virtual bool isOpen() const = 0;
    virtual size_t getFileIndex() const = 0;
};
}  // namespace rgl