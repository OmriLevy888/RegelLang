#include "common/Formatter.hpp"
#include "common/source-objects/SourceLine.hpp"
#include "lexer/Token.hpp"

namespace rgl {
SourceLine::SourceLine(const std::string &repr) : m_repr(repr) {}
SourceLine::SourceLine(std::string &&repr) : m_repr(std::move(repr)) {}
SourceLine::SourceLine(std::string &&repr,
                       const std::vector<TokenValuePair> &tokens)
    : m_repr(std::move(repr)) {
  m_locations.reserve(tokens.size());
  for (const auto &token : tokens) {
    m_locations.push_back(token.m_token.sourceLocation());
  }
}
SourceLine::SourceLine(std::string &&repr,
                       const std::vector<TokenValuePair> &tokens,
                       const size_t lineNo)
    : m_repr(std::move(repr)) {
  m_locations.reserve(tokens.size());
  for (auto token : tokens) {
    if (lineNo != token.m_token.sourceLocation().m_line) {
      continue;
    }

    m_locations.push_back(token.m_token.sourceLocation());
  }
}

std::string SourceLine::pointAt(const SourceRange &range) const {
  const std::string_view repr{m_repr.c_str() + range.m_startOffset,
                              range.m_endOffset - range.m_startOffset};

  size_t numSpaces = repr.cbegin() - m_repr.c_str();
  size_t numUnderline = repr.length() - 1;
  std::string spaces(numSpaces, ' ');
  std::string underline(numUnderline, '^');

  return spaces + underline;
}

std::string SourceLine::toString() const {
  return Formatter("SourceLine<{}>", m_repr);
}
}; // namespace rgl
