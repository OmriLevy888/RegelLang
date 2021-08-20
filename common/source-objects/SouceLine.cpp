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

std::pair<std::string, std::string>
SourceLine::pointAt(const SourceLocation &sourceLocation) const {
  const std::string_view repr{m_repr.c_str() + sourceLocation.m_reprStartIdx,
                              sourceLocation.m_reprLen};

  size_t numSpaces = repr.cbegin() - m_repr.c_str();
  size_t numUnderline = repr.length();
  size_t numHandle = numSpaces + (numUnderline / 2);
  std::string spaces(numSpaces, ' ');
  std::string underline(numUnderline, '^');
  std::string handle(numHandle, '_');

  return {spaces + underline, handle + "|"};
}

std::string SourceLine::toString() const {
  return Formatter("SourceLine<{}>", m_repr);
}
}; // namespace rgl
