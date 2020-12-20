#pragma once
#include "common/Core.hpp"
#include "lexer/ITokenGenerator.hpp"
#include <vector>

// TODO: allo passing values for the dummy token generator when writing parser
// tests

namespace rgl {
class DummyTokenGenerator : public ITokenGenerator {
public:
  DummyTokenGenerator(const std::vector<Token> &tokens)
      : m_yieldedEOF(false), m_index(0), m_tokens(tokens) {}
  DummyTokenGenerator(std::vector<Token> &&tokens)
      : m_yieldedEOF(false), m_index(0), m_tokens(std::move(tokens)) {}

  TokenValuePair getNext() override {
    if (m_yieldedEOF || m_tokens.size() == 0) {
      return Token(TokenType::t_eof);
    }

    if (m_index + 1 >= m_tokens.size() || m_tokens[m_index].isEOF()) {
      m_yieldedEOF = true;
    }

    return m_tokens[m_index++];
  }

  std::string toString() const override {
    return "DummyTokenGenerator<size: " + std::to_string(m_tokens.size()) + ">";
  }

private:
  bool m_yieldedEOF;
  size_t m_index;
  std::vector<Token> m_tokens;
};
} // namespace rgl
