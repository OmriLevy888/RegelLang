#include "TextSourceStream.h"

#include <algorithm>

namespace rgl {
int TextSourceStream::peek() {
    if (m_pos >= m_end) return EOF;
    return m_sourceText[m_pos];
}
int TextSourceStream::read() {
    if (m_pos >= m_end) return EOF;
    return m_sourceText[m_pos++];
}
bool TextSourceStream::readLine(std::string &line) {
    size_t line_end_index = m_sourceText.find('\n', m_pos);
    if (line_end_index == std::string::npos) {
        line = m_sourceText.substr(m_pos);
        bool ret = m_pos != m_end;
        m_pos = m_end;
        return ret;
    }
    line = m_sourceText.substr(m_pos, line_end_index - m_pos);
    m_pos = line_end_index + 1;
    return true;
}
size_t TextSourceStream::tell() const {
    return m_pos;
}
size_t TextSourceStream::seek(off_t offset, std::ios_base::seekdir whence) {
    int64_t result;
    switch (whence) {
    case std::ios_base::beg: result = 0 + offset; break;
    case std::ios_base::cur: result = static_cast<int64_t>(m_pos) + offset; break;
    case std::ios_base::end: result = static_cast<int64_t>(m_end) + offset; break;
    }
    if (result > m_end) result = m_end;
    else if (result < 0) result = 0;
    m_pos = result;
    return tell();
}

std::string TextSourceStream::toString() const {
    return "TextSourceStream<pos: " + std::to_string(m_pos) + ", m_end:" + std::to_string(m_end) 
            + ">";
}
}  // namespace rgl