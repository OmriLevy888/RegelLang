#include "lexer/DummyTokenGenerator.hpp"

namespace rgl {
TokenValuePair DummyTokenGenerator::getNext() {
  if (m_yieldedEOF || m_tokens.size() == 0) {
    if (m_tokens.size() != 0 &&
        TokenType::t_eof == m_tokens[m_tokens.size() - 1]) {
      return m_tokens[m_tokens.size() - 1];
    }
    return Token(TokenType::t_eof);
  }

  if (m_index + 1 >= m_tokens.size() || m_tokens[m_index].m_token.isEOF()) {
    m_yieldedEOF = true;
  }

  TokenValuePair tvp = m_tokens[m_index];

  return m_tokens[m_index++];
}
}; // namespace rgl
