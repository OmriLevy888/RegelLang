#include "lexer/Lexer.h"

namespace rgl {
Token Lexer::getNext() {
    if ((0 == m_project->m_files[m_sourceStream->getFileIndex()].m_lines.size())
        || (!skipWhiteSpace())) {
        // new line
        std::string line;
        if (!m_sourceStream->readLine(line)) {  // eof
            addLine("");
            Token eof(TokenType::t_eof, genTokenId());
            addToken(eof);
            return eof;
        }
        addLine(std::move(line));
        m_pos = 0;
    }

    // lexing happens here...
    /// lexKeyword...
    // lexLiteral...
    // lexSpecialSymbol...
    // lexOperator...
    // lexIdentifier...

    // if reached this line, an error has occured
    return Token(TokenType::t_eof);
}

bool Lexer::skipWhiteSpace() {
    size_t currLineLength = m_currLine->m_repr.length();
    for (; m_pos < currLineLength; m_pos++) {
        if ((m_pos != ' ') && (m_pos != '\t')) {
            return true;
        } 
    }
    return false;
}

void Lexer::addLine(std::string &&line) {
    m_project->lock();
    m_currLineIdx = m_project->m_files[m_sourceStream->getFileIndex()].m_lines.size();
    m_project->m_files[m_sourceStream->getFileIndex()].m_lines.emplace_back(std::move(line));
    m_currLine = m_project->m_files[m_sourceStream->getFileIndex()].m_lines.begin();// + currLineIdx;
    m_project->release();
}

void Lexer::addToken(const Token &token) {
    m_currLine->m_tokens.push_back(token);
}

uint64_t Lexer::genTokenId() const {
    return ((m_sourceStream->getFileIndex() & 0xffffff) << 5)
        | ((m_currLineIdx & 0xffffff) << 2)
        | (m_currLine->m_tokens.size() & 0xffff);
}
}