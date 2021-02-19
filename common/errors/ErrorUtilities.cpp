#include "common/errors/ErrorUtilities.hpp"
#include "common/Formatter.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "lexer/TokenCollection.hpp"

namespace rgl {
std::string tokenToString(Token tok,
                          std::shared_ptr<SourceProject> sourceProject) {
  if (TokenType::t_eof == tok) {
    return "end of file";
  }

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

  const size_t fileNo = tok.getFileNo();
  const size_t lineNo = tok.getLineNo();
  const auto &file = sourceProject->m_files[fileNo];
  const auto &line = file.m_lines[lineNo];
  const auto [pointerTopStr, pointerBottomStr] =
      line.pointAt(tok.getTokenIdx());
  static const std::string pointerBottomExtStr(4, '_');

  static const std::string spacesStr(4, ' ');
  static const std::string blankLinNoStr(11, ' ');
  const auto lineNoStr = padLeft(padLeft(lineNo, 7), 11, ' ');

  std::string formatted =
      Formatter("{}|{}{}\n{}|{}{}\n{}|{}{}", lineNoStr, spacesStr, line.m_repr,
                blankLinNoStr, spacesStr, pointerTopStr, blankLinNoStr,
                pointerBottomExtStr, pointerBottomStr);

  if (lineNo < file.m_lines.size() - 1) { // add next line
    const auto nextLineNoStr = padLeft(padLeft(lineNo + 1, 7), 11, ' ');
    formatted = Formatter("{}\n{}|{}{}", formatted, nextLineNoStr, spacesStr,
                          file.m_lines[fileNo + 1].m_repr);
  }

  if (lineNo > 0) { // add previous line
    const auto prevLineNoStr = padLeft(padLeft(lineNo - 1, 7), 11, ' ');
    formatted = Formatter("{}|{}{}\n{}", prevLineNoStr, spacesStr,
                          file.m_lines[lineNo - 1].m_repr, formatted);
  }

  if (lineNo > 1) { // add line before previous line
    const auto prevPrevLineNoStr = padLeft(padLeft(lineNo - 2, 7), 11, ' ');
    formatted = Formatter("{}|{}{}\n{}", prevPrevLineNoStr, spacesStr,
                          file.m_lines[lineNo - 2].m_repr, formatted);
  }

  return formatted;
}
std::string pointAt(const std::unique_ptr<TokenCollection> &tokens) {
  return pointAt(tokens->getCurr(), tokens->getSourceProject());
}
}; // namespace rgl
