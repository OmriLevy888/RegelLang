#include "FileSourceStream.h"

namespace rgl {
int FileSourceStream::peek() {
    return m_ifstream.peek();
}
int FileSourceStream::read() {
    return m_ifstream.get();
}
bool FileSourceStream::readLine(std::string &line) {
    return static_cast<bool>(std::getline(m_ifstream, line));
}
size_t FileSourceStream::tell() const {
    return m_ifstream.tellg();
}
size_t FileSourceStream::seek(off_t offset, std::ios_base::seekdir whence) {
    m_ifstream.seekg(offset, whence);
    return tell();
}

std::string FileSourceStream::toString() const {
    return "FileSourceStream<path:\"" + m_filePath + "\">";
}
}  // namespace rgl