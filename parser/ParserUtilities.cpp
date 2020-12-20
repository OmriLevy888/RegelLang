#include "parser/ParserUtilities.hpp"
#include "lexer/Token.hpp"

namespace rgl {
bool ParserUtilities::isIdentifier(const Token &tok) {
  return tok.getTokenType() == TokenType::t_identifier;
}

bool ParserUtilities::isLiteral(const Token &tok) {
  return isIntLiteral(tok) || isRealLiteral(tok) || isTextLiteral(tok) ||
         isBooleanLiteral(tok);
}

bool ParserUtilities::isIntLiteral(const Token &tok) {
  switch (tok.getTokenType()) {
  case TokenType::t_int8_literal:
  case TokenType::t_int16_literal:
  case TokenType::t_int32_literal:
  case TokenType::t_int64_literal:
  case TokenType::t_uint8_literal:
  case TokenType::t_uint16_literal:
  case TokenType::t_uint32_literal:
  case TokenType::t_uint64_literal:
    return true;
  default:
    return false;
  }
}

bool ParserUtilities::isRealLiteral(const Token &tok) {
  return tok == TokenType::t_float_literal ||
         tok == TokenType::t_double_literal;
}

bool ParserUtilities::isTextLiteral(const Token &tok) {
  return tok == TokenType::t_char_literal || tok == TokenType::t_string_literal;
}

bool ParserUtilities::isBooleanLiteral(const Token &tok) {
  return tok == TokenType::t_boolean;
}
}; // namespace rgl
