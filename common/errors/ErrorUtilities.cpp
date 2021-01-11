#include "common/errors/ErrorUtilities.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "lexer/TokenCollection.hpp"

namespace rgl {
std::string tokenToString(Token tok,
                          std::shared_ptr<SourceProject> sourceProject) {
  if (nullptr == sourceProject) {
    return "";
  }

  const auto &line =
      sourceProject->m_files[tok.getFileNo()].m_lines[tok.getLineNo()];
  const auto begin = line.m_repr.cbegin() + tok.getReprStartIdx();
  return {begin, begin + tok.getReprLen()};
}
std::string tokenToString(const std::unique_ptr<TokenCollection> &tokens) {
  return tokenToString(tokens->getCurr(), tokens->getSourceProject());
}

std::string pointAt(Token tok, std::shared_ptr<SourceProject> sourceProject) {
  if (nullptr == sourceProject) {
    return "";
  }

  const auto &line =
      sourceProject->m_files[tok.getFileNo()].m_lines[tok.getLineNo()];
  return line.pointAt(tok.getTokenIdx());
}
std::string pointAt(const std::unique_ptr<TokenCollection> &tokens) {
  return pointAt(tokens->getCurr(), tokens->getSourceProject());
}
}; // namespace rgl
