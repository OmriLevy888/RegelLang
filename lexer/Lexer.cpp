#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include <algorithm>
#include <array>
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

  if (lexComment())
    return getNextImpl();

  Token ret;
  // lexing happens here...
  if (lexKeyword(ret)) {
    return ret;
  } else if (lexIdentifier(ret)) {
    return ret;
  } else if (lexLiteral(ret)) {
    return ret;
  } else if (lexOperator(ret)) {
    return ret;
  } else if (lexSpecialCharacter(ret)) {
    return ret;
  }

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

bool Lexer::lexComment() {
  if (m_state == LexerState::multilineComment) {
    for (; m_pos < m_currLine->m_repr.size() - 1; m_pos++) {
      const char curr = m_currLine->m_repr[m_pos];
      const char next = m_currLine->m_repr[m_pos + 1];
      if (curr == '*' && next == '/') {
        m_state = LexerState::normal;
        m_pos += 2;
        return true;
      }
    }
    m_pos = m_currLine->m_repr.size();
    return true;
  }

  if (m_pos + 2 > m_currLine->m_repr.size())
    return false;

  const char first = m_currLine->m_repr[m_pos];
  const char second = m_currLine->m_repr[m_pos + 1];

  if (first == '/' &&
      second == '/') { // found a single line comment till the end of the line
    m_pos = m_currLine->m_repr.size();
    return true;
  } else if (first == '/' &&
             second == '*') { // found the start of a multiline comment
    m_state = LexerState::multilineComment;
    return lexComment();
  }

  return false;
}

bool Lexer::lexKeyword(Token &ret) {
  static std::map<std::string, TokenType> keywords = {
      {"true", TokenType::t_true},
      {"false", TokenType::t_false},
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
  if (lexCharLiteral(ret))
    return true;
  else if (lexStringLiteral(ret))
    return true;
  else if (lexRealLiteral(ret))
    return true;
  else if (lexIntLiteral(ret))
    return true;
  return false;
}

bool Lexer::lexCharLiteral(Token &ret) {
  if (m_currLine->m_repr[m_pos] != '\'')
    return false;
  const size_t originalPos = m_pos++;
  if (!lexCharacter()) {
    m_pos = originalPos;
    return false;
  }
  if (m_currLine->m_repr[m_pos++] != '\'') {
    m_pos = originalPos;
    return false;
  }
  ret = makeToken(TokenType::t_char_literal, originalPos, m_pos - originalPos);
  return true;
}

bool Lexer::lexStringLiteral(Token &ret) {
  if (m_currLine->m_repr[m_pos] != '\"')
    return false;
  const size_t originalPos = m_pos++;
  while (m_currLine->m_repr[m_pos] != '\"') {
    if (!lexCharacter()) {
      m_pos = originalPos;
      return false;
    }
  }
  m_pos++;
  ret =
      makeToken(TokenType::t_string_literal, originalPos, m_pos - originalPos);
  return true;
}

bool Lexer::lexIntLiteral(Token &ret) {
  static std::array<char, 16> digits = {'0', '1', '2', '3', '4', '5', '6', '7',
                                        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

  const size_t originalPos = m_pos;
  uint8_t base = 10;
  if (m_currLine->m_repr[m_pos] == '0' &&
      m_pos + 1 < m_currLine->m_repr.size()) {
    switch (m_currLine->m_repr[m_pos + 1]) { // check base
    case 'b':
    case 'B':
      base = 2;
      break;
    case 'o':
    case 'O':
      base = 8;
      break;
    case 'x':
    case 'X':
      base = 16;
      break;
    default:
      m_pos -= 2; // to cancel the addition
      break;
    }
    m_pos += 2;
  }

  const size_t numStartPos = m_pos;
  for (; m_pos != m_currLine->m_repr.size();
       m_pos++) { // search until found invalid digit character
    char curr = m_currLine->m_repr[m_pos];
    if (std::distance(digits.cbegin(), std::find(digits.cbegin(), digits.cend(),
                                                 curr)) >= base) {
      break;
    }
  }

  if (numStartPos == m_pos) { // if found no valid digits
    m_pos = originalPos;
    return false;
  } else if (m_pos >= m_currLine->m_repr.size()) { // this means we exited the
                                                   // loop cause the line ended
    ret =
        makeToken(TokenType::t_int32_literal, originalPos, m_pos - originalPos);
    return true;
  }

  if (m_currLine->m_repr[m_pos] == 'i' ||
      m_currLine->m_repr[m_pos] == 'u') { // check literal type
    const char type = m_currLine->m_repr[m_pos];
    if (m_currLine->m_repr.size() <=
        m_pos + 1) { // if the type character is the last character
      m_pos++; // m_pos now points outside the current line, so we move to the
               // next line
      switch (type) {
      case 'i':
        ret = makeToken(TokenType::t_int32_literal, originalPos,
                        m_pos - originalPos);
        return true;
      case 'u':
        ret = makeToken(TokenType::t_uint32_literal, originalPos,
                        m_pos - originalPos);
        return true;
      }
    }

    const char first = m_currLine->m_repr[m_pos + 1];
    m_pos++;
    if (m_currLine->m_repr.size() <=
        m_pos + 1) { // if there is only one character after the type character
      if (first == '8') {
        m_pos++; // m_pos now points outside the current line, so we move to the
                 // next line
        switch (type) { // check which is the correct type
        case 'i':
          ret = makeToken(TokenType::t_int8_literal, originalPos,
                          m_pos - originalPos);
          return true;
        case 'u':
          ret = makeToken(TokenType::t_uint8_literal, originalPos,
                          m_pos - originalPos);
          return true;
        }
      } else {
        switch (type) { // check which is the correct type
        case 'i':
          ret = makeToken(TokenType::t_int32_literal, originalPos,
                          m_pos - originalPos);
          return true;
        case 'u':
          ret = makeToken(TokenType::t_uint32_literal, originalPos,
                          m_pos - originalPos);
          return true;
        }
      }
    }

    const char second = m_currLine->m_repr[m_pos + 1];
    m_pos++;
    TokenType tokType = TokenType::t_err;
    uint32_t startIdx = originalPos;
    uint16_t reprLen = 0;
    switch (first) {
    case '8':
      tokType = (type == 'i') ? (TokenType::t_int8_literal)
                              : (TokenType::t_uint8_literal);
      reprLen = m_pos - startIdx;
      break;
    case '1':
      if (second != '6') {
        m_pos = originalPos;
        return false;
      }
      tokType = (type == 'i') ? (TokenType::t_int16_literal)
                              : (TokenType::t_uint16_literal);
      m_pos++;
      reprLen = m_pos - startIdx;
      break;
    case '3':
      if (second != '2') {
        m_pos = originalPos;
        return false;
      }
      tokType = (type == 'i') ? (TokenType::t_int32_literal)
                              : (TokenType::t_uint32_literal);
      m_pos++;
      reprLen = m_pos - startIdx;
      break;
    case '6':
      if (second != '4') {
        m_pos = originalPos;
        return false;
      }
      tokType = (type == 'i') ? (TokenType::t_int64_literal)
                              : (TokenType::t_uint64_literal);
      m_pos++;
      reprLen = m_pos - startIdx;
      break;
    default:
      tokType = (type == 'i') ? (TokenType::t_int32_literal)
                              : (TokenType::t_uint32_literal);
      m_pos--;
      reprLen = m_pos - startIdx;
      break;
    }
    ret = makeToken(tokType, startIdx, reprLen);
    return true;
  }

  ret = makeToken(TokenType::t_int32_literal, originalPos, m_pos - originalPos);
  return true;
}

bool Lexer::lexRealLiteral(Token &ret) {
  bool foundDot = false;
  const size_t originalPos = m_pos;

  for (; m_pos < m_currLine->m_repr.size(); m_pos++) {
    if (!std::isdigit(m_currLine->m_repr[m_pos]))
      break;
  }

  if (m_currLine->m_repr[m_pos] == '.') { // found dot, look for fraction
    foundDot = true;
    for (m_pos++; m_pos < m_currLine->m_repr.size(); m_pos++) {
      if (!std::isdigit(m_currLine->m_repr[m_pos]))
        break;
    }
  }

  TokenType tokType = TokenType::t_double_literal;
  if (m_pos < m_currLine->m_repr.size()) { // check for f or d suffix
    switch (m_currLine->m_repr[m_pos++]) {
    case 'd':
      ret = makeToken(TokenType::t_double_literal, originalPos,
                      m_pos - originalPos);
      return true;
    case 'f':
      ret = makeToken(TokenType::t_float_literal, originalPos,
                      m_pos - originalPos);
      return true;
    default:
      m_pos--;
      break;
    }
  }

  if (!foundDot) { // if there is no real literal suffix or a decimal dot, this
                   // is not a real literal
    m_pos = originalPos;
    return false;
  } else if (m_pos ==
             originalPos +
                 1) { // if only found a dot, this is not a real literal
    m_pos = originalPos;
    return false;
  }

  // if found dot, the default type is double literal
  ret =
      makeToken(TokenType::t_double_literal, originalPos, m_pos - originalPos);
  return true;
}

bool Lexer::lexCharacter() {
  const char curr = m_currLine->m_repr[m_pos];
  if ('\\' == curr) {
    if (m_pos >= m_currLine->m_repr.length() - 1)
      return false;

    const char second = m_currLine->m_repr[m_pos + 1];
    m_pos += 2;
    switch (second) {
    case 'a':
      return true;
    case 'b':
      return true;
    case 'e':
      return true;
    case 'f':
      return true;
    case 'n':
      return true;
    case 'r':
      return true;
    case 't':
      return true;
    case 'v':
      return true;
    case '\\':
      return true;
    case '\'':
      return true;
    case '\"':
      return true;
    case '?':
      return true;
    }
    m_pos -= 2;
    if (second != 'x' && second != 'X')
      return false;
    if (m_pos >= m_currLine->m_repr.length() - 3)
      return false;
    const char third = m_currLine->m_repr[m_pos + 2];
    const char fourth = m_currLine->m_repr[m_pos + 3];
    if (isHex(third) && isHex(fourth)) {
      m_pos += 4;
      return true;
    }
    return false;
  }
  if (std::isgraph(curr))
    m_pos++;
  return std::isgraph(curr);
}

bool Lexer::isHex(const char value) const noexcept {
  return std::isdigit(value) || ('a' <= value && value <= 'f') ||
         ('A' <= value && value <= 'F');
}
} // namespace rgl
