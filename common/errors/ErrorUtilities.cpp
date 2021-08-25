#include "common/errors/ErrorUtilities.hpp"
#include "common/Formatter.hpp"
#include "common/source-objects/SourceRange.hpp"
#include "common/source-objects/SourceProject.hpp"
#include "lexer/TokenCollection.hpp"

namespace rgl {
std::string tokenToString(Token tok) {
  if (TokenType::t_eof == tok) {
    return "end of file";
  }

  const auto sourceLocation = tok.sourceLocation();
  if (sourceLocation.m_file >= SourceProject::get().files().size()) {
    return "";
  }
  const auto &file = SourceProject::get().files()[sourceLocation.m_file];
  if (sourceLocation.m_line >= file.m_lines.size()) {
    return "";
  }
  const auto &line = file.m_lines[sourceLocation.m_line];
  const auto begin = line.cbegin() + sourceLocation.m_reprStartIdx;
  return {begin, begin + sourceLocation.m_reprLen};
}
std::string tokenToString(const std::unique_ptr<TokenCollection> &tokens) {
  return tokenToString(tokens->getCurr());
}

std::string pointAt(Token tok) {
  return pointAt(SourceRange(tok.sourceLocation()));
}
std::string pointAt(const std::unique_ptr<TokenCollection> &tokens) {
  return pointAt(tokens->getCurr());
}
std::string pointAt(const SourceRange &range) {
  const size_t fileNo = range.m_file;
  const size_t lineNo = range.m_startLine;
  const auto &file = SourceProject::get().files()[fileNo];
  const auto &line = file.m_lines[lineNo];
  const auto pointerTopStr = line.pointAt(range);

  static const std::string spacesStr(1, ' ');
  static const std::string blankLinNoStr(11, ' ');
  const auto lineNoStr = padLeft(padLeft(lineNo, 7), 11, ' ');

  std::string formatted =
      Formatter("{}|{}{}\n{}|{}{}", lineNoStr, spacesStr, line.repr(),
                blankLinNoStr, spacesStr, pointerTopStr);

  if (lineNo < file.m_lines.size() - 1) { // add next line
    const auto nextLineNoStr = padLeft(padLeft(lineNo + 1, 7), 11, ' ');
    formatted = Formatter("{}\n{}|{}{}", formatted, nextLineNoStr, spacesStr,
                          file.m_lines[lineNo + 1].repr());
  }

  if (lineNo > 0) { // add previous line
    const auto prevLineNoStr = padLeft(padLeft(lineNo - 1, 7), 11, ' ');
    formatted = Formatter("{}|{}{}\n{}", prevLineNoStr, spacesStr,
                          file.m_lines[lineNo - 1].repr(), formatted);
  }

  if (lineNo > 1) { // add line before previous line
    const auto prevPrevLineNoStr = padLeft(padLeft(lineNo - 2, 7), 11, ' ');
    formatted = Formatter("{}|{}{}\n{}", prevPrevLineNoStr, spacesStr,
                          file.m_lines[lineNo - 2].repr(), formatted);
  }

  return formatted;
}
}; // namespace rgl
