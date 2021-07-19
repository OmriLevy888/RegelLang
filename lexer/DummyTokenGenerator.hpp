#pragma once
#include "common/Core.hpp"
#include "lexer/ITokenGenerator.hpp"
#include <memory>
#include <variant>
#include <vector>

namespace rgl {
class DummyTokenGenerator : public ITokenGenerator {
public:
  DummyTokenGenerator(const std::vector<TokenValuePair> &tokens)
      : m_yieldedEOF(false), m_index(0), m_tokens(tokens) {}
  DummyTokenGenerator(std::vector<TokenValuePair> &&tokens)
      : m_yieldedEOF(false), m_index(0), m_tokens(std::move(tokens)) {}

  TokenValuePair getNext() override;

  std::string toString() const override {
    return "DummyTokenGenerator<size: " + std::to_string(m_tokens.size()) + ">";
  }

private:
  bool m_yieldedEOF;
  size_t m_index;
  std::vector<TokenValuePair> m_tokens;
};
} // namespace rgl
