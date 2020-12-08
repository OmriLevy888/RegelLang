#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include <algorithm>
#include <cctype>
#include <string>

namespace rgl {
Token Lexer::getNext() {
  if (m_yieldedEof) {
    return m_eof;
  }
  Token ret = getNextImpl();
  addToken(ret);
  return ret;
}
Token Lexer::makeToken(TokenType type, uint32_t reprStartIdx,
                       uint16_t reprLen) const {
  return Token(type, reprStartIdx, reprLen, m_sourceStream->getFileIndex(),
               m_currLineIdx, m_currTokenIdx);
}

Token Lexer::getNextImpl() {
  if ((0 ==
       m_project->m_files[m_sourceStream->getFileIndex()].m_lines.size()) ||
      (!skipWhiteSpace())) {
    // new line
    std::string line;
    if (!m_sourceStream->readLine(line)) { // eof
      m_file->m_lines.emplace_back(line);
      m_eof = makeToken(TokenType::t_eof);
      m_yieldedEof = true;
      return m_eof;
    }

    m_file->m_lines.emplace_back(line);
    m_currTokenIdx = 0;
    m_currLineIdx = m_file->m_lines.size() - 1;
    m_pos = 0;
    m_currLine = m_file->m_lines.begin() + m_currLineIdx;
    return getNextImpl();
  }

  Token ret;
  // lexing happens here...
  if (lexKeyword(ret)) {
    return ret;
  } else if (lexIdentifier(ret)) {
    return ret;
  } else if (lexSpecialCharacter(ret)) {
    return ret;
  } else if (lexOperator(ret)) {
    return ret;
  }
  // lexLiteral...

  // if reached this line, an error has occured
  return Token(TokenType::t_err);
}

bool Lexer::skipWhiteSpace() {
  size_t currLineLength = m_currLine->m_repr.length();
  for (; m_pos < currLineLength; m_pos++) {
    if ((m_currLine->m_repr[m_pos] != ' ') &&
        (m_currLine->m_repr[m_pos] != '\t')) {
      return true;
    }
  }
  return false;
}

void Lexer::addToken(const Token &token) {
  m_currLine->m_tokens.push_back(token);
  m_currTokenIdx++;
}

bool Lexer::lexKeyword(Token &ret) {
  static std::map<std::string, TokenType> keywords = {
      {"return", TokenType::t_return},
      {"yield", TokenType::t_yield},
      {"let", TokenType::t_let},
      {"var", TokenType::t_var},
      {"as", TokenType::t_as},
      {"if", TokenType::t_if},
      {"elif", TokenType::t_elif},
      {"else", TokenType::t_else},
      {"for", TokenType::t_for},
      {"while", TokenType::t_while},
      {"times", TokenType::t_times},
      {"break", TokenType::t_break},
      {"continue", TokenType::t_continue},
      {"switch", TokenType::t_switch},
      {"func", TokenType::t_func}};

  const auto start = m_currLine->m_repr.cbegin() + m_pos;
  auto it = start + 1;
  for (; it != m_currLine->m_repr.cend(); it++) {
    if (!std::isalpha(*it)) {
      break;
    }
  }

  std::string possibleKeyword{start, it};
  TokenType type = keywords[possibleKeyword];
  if (TokenType::t_err == type) {
    return false;
  }
  size_t len = it - start;
  ret = makeToken(type, m_pos, len);
  m_pos += len;
  return true;
}

bool Lexer::lexIdentifier(Token &ret) {
  const char curr = m_currLine->m_repr[m_pos];
  if (!std::isalpha(curr) && '_' != curr) {
    return false;
  }
  const auto start = m_currLine->m_repr.cbegin() + m_pos;
  auto it = start + 1;
  for (; it != m_currLine->m_repr.cend(); it++) {
    if (!std::isalnum(*it) && '_' != *it) {
      break;
    }
  }

  if (it == start + 1 && *start == '_') {
    return false;
  }

  size_t len = it - start;
  ret = makeToken(TokenType::t_identifier, m_pos, len);
  m_pos += len;
  return true;
}

bool Lexer::lexSpecialCharacter(Token &ret) {
  const char curr = m_currLine->m_repr[m_pos];
  switch (m_currLine->m_repr[m_pos]) {
  case '_':
    ret = makeToken(TokenType::t_underscore, m_pos, 1);
    break;
  case ';':
    ret = makeToken(TokenType::t_semicolon, m_pos, 1);
    break;
  case ':':
    ret = makeToken(TokenType::t_colon, m_pos, 1);
    break;
  case '#':
    ret = makeToken(TokenType::t_pound, m_pos, 1);
    break;
  case '{':
    ret = makeToken(TokenType::t_open_bracket, m_pos, 1);
    break;
  case '}':
    ret = makeToken(TokenType::t_close_bracket, m_pos, 1);
    break;
  case '(':
    ret = makeToken(TokenType::t_open_paren, m_pos, 1);
    break;
  case ')':
    ret = makeToken(TokenType::t_close_paren, m_pos, 1);
    break;
  case '[':
    ret = makeToken(TokenType::t_open_square, m_pos, 1);
    break;
  case ']':
    ret = makeToken(TokenType::t_close_square, m_pos, 1);
    break;
  case '.':
    ret = makeToken(TokenType::t_dot, m_pos, 1);
    break;
  case '?':
    ret = makeToken(TokenType::t_question_mark, m_pos, 1);
    break;
  case ',':
    ret = makeToken(TokenType::t_comma, m_pos, 1);
    break;
  default:
    return false;
  }
  m_pos++;
  return true;
}

bool Lexer::lexOperator(Token &ret) {
  static std::map<std::string, TokenType> keywords = {
      {"+", TokenType::t_plus},
      {"-", TokenType::t_minus},
      {"*", TokenType::t_asterisk},
      {"/", TokenType::t_forward_slash},
      {"++", TokenType::t_plus_plus},
      {"--", TokenType::t_minus_minus},
      {"=", TokenType::t_equal},
      {"!", TokenType::t_exclamation},
      {"^", TokenType::t_caret},
      {"|", TokenType::t_pipe},
      {"&", TokenType::t_ampersand},
      {"==", TokenType::t_equal_equal},
      {"!=", TokenType::t_not_equal},
      {">", TokenType::t_greater_than},
      {"<", TokenType::t_lesser_than},
      {">=", TokenType::t_greater_equal},
      {"<=", TokenType::t_lesser_equal},
      {"+=", TokenType::t_plus_equal},
      {"-=", TokenType::t_minus_equal},
      {"*=", TokenType::t_asterisk_equal},
      {"/=", TokenType::t_forward_slash_equal},
      {"^=", TokenType::t_caret_equal},
      {"|=", TokenType::t_pipe_equal},
      {"&=", TokenType::t_ampersand_equal},
      {">>", TokenType::t_shift_right},
      {"<<", TokenType::t_shift_left},
      {">>=", TokenType::t_shift_right_equal},
      {"<<=", TokenType::t_shift_left_equal},
      {"=>", TokenType::t_arrow}};
  static const size_t MAX_OPERATOR_LEN =
      3; // Change this when new operators longer than 3 characters are
         // introduces, though this should probably never happen
  const size_t lenLeft = m_currLine->m_repr.size() - m_pos;
  const size_t len = std::min(MAX_OPERATOR_LEN, lenLeft);
  std::string curr = m_currLine->m_repr.substr(m_pos, len);
  while (0 != curr.size()) {
    const auto match = keywords.find(curr);
    if (keywords.cend() != match) {
      m_pos += curr.size();
      ret = match->second;
      return true;
    }
    curr = curr.substr(0, curr.size() - 1);
  }
  return false;
}

bool Lexer::lexLiteral(Token &ret) {
  // TODO: implement this
  return false;
}
} // namespace rgl
