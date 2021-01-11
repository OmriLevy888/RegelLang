#pragma once
#include "common/Core.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "lexer/ITokenGenerator.hpp"
#include <memory>
#include <variant>
#include <vector>

namespace rgl {
class DummyTokenGenerator : public ITokenGenerator {
public:
  DummyTokenGenerator(const std::vector<TokenValuePair> &tokens,
                      std::shared_ptr<SourceProject> sourceProject = nullptr)
      : m_yieldedEOF(false), m_index(0), m_tokens(tokens),
        m_sourceProject(sourceProject) {}
  DummyTokenGenerator(std::vector<TokenValuePair> &&tokens,
                      std::shared_ptr<SourceProject> sourceProject = nullptr)
      : m_yieldedEOF(false), m_index(0), m_tokens(std::move(tokens)),
        m_sourceProject(sourceProject) {}

  TokenValuePair getNext() override {
    if (m_yieldedEOF || m_tokens.size() == 0) {
      return Token(TokenType::t_eof);
    }

    if (m_index + 1 >= m_tokens.size() || m_tokens[m_index].m_token.isEOF()) {
      m_yieldedEOF = true;
    }

    TokenValuePair tvp = m_tokens[m_index];

    return m_tokens[m_index++];
  }

  virtual std::shared_ptr<SourceProject> getSourceProject() const override {
    return m_sourceProject;
  }

  std::string toString() const override {
    return "DummyTokenGenerator<size: " + std::to_string(m_tokens.size()) + ">";
  }

private:
  bool m_yieldedEOF;
  size_t m_index;
  std::vector<TokenValuePair> m_tokens;

  std::shared_ptr<SourceProject> m_sourceProject;
};
} // namespace rgl
