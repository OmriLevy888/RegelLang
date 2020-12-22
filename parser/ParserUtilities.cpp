#include "parser/ParserUtilities.hpp"
#include "lexer/Token.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"

#include <unordered_map>

namespace rgl {
bool ParserUtilities::isIdentifier(const Token &tok) {
  return tok == TokenType::t_identifier;
}
bool ParserUtilities::isLiteral(const Token &tok) {
  return isIntLiteral(tok) || isRealLiteral(tok) || isTextLiteral(tok) ||
         isBooleanLiteral(tok);
}
bool ParserUtilities::isIntLiteral(const Token &tok) {
  switch (tok) {
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

bool ParserUtilities::isSignedIntLiteral(const Token &tok) {
  return tok == TokenType::t_int8_literal ||
         tok == TokenType::t_int16_literal ||
         tok == TokenType::t_int32_literal || tok == TokenType::t_int64_literal;
}
bool ParserUtilities::isUnsignedIntLiteral(const Token &tok) {
  return tok == TokenType::t_uint8_literal ||
         tok == TokenType::t_uint16_literal ||
         tok == TokenType::t_uint32_literal ||
         tok == TokenType::t_uint64_literal;
}

bool ParserUtilities::isBinOp(const Token &tok) {
  switch (tok) {
  case TokenType::t_dot:
  case TokenType::t_plus:
  case TokenType::t_minus:
  case TokenType::t_asterisk:
  case TokenType::t_forward_slash:
  case TokenType::t_ampersand:
  case TokenType::t_pipe:
  case TokenType::t_caret:
  case TokenType::t_equal_equal:
  case TokenType::t_not_equal:
  case TokenType::t_lesser_than:
  case TokenType::t_greater_than:
  case TokenType::t_lesser_equal:
  case TokenType::t_greater_equal:
    return true;
  default:
    return false;
  }
}
uint8_t ParserUtilities::tokToPrecedence(const Token &tok) {
  static std::unordered_map<TokenType, uint8_t> precendenceMap{
      {TokenType::t_dot, 0},

      {TokenType::t_asterisk, 1},     {TokenType::t_forward_slash, 1},

      {TokenType::t_plus, 2},         {TokenType::t_minus, 2},

      {TokenType::t_equal_equal, 3},  {TokenType::t_not_equal, 3},
      {TokenType::t_lesser_than, 3},  {TokenType::t_greater_than, 3},
      {TokenType::t_lesser_equal, 3}, {TokenType::t_greater_equal, 3},

      {TokenType::t_ampersand, 4},    {TokenType::t_caret, 5},
      {TokenType::t_pipe, 6},
  };

  return precendenceMap[tok];
}
BinOpType ParserUtilities::tokToBinOpType(const Token &tok) {
  static std::unordered_map<TokenType, BinOpType> binOpMap{
      {TokenType::t_dot, BinOpType::b_dot},

      {TokenType::t_asterisk, BinOpType::b_asterisk},
      {TokenType::t_forward_slash, BinOpType::b_forward_slash},

      {TokenType::t_plus, BinOpType::b_plus},
      {TokenType::t_minus, BinOpType::b_minus},

      {TokenType::t_equal_equal, BinOpType::b_equal_equal},
      {TokenType::t_not_equal, BinOpType::b_not_equal},
      {TokenType::t_lesser_than, BinOpType::b_lesser_than},
      {TokenType::t_greater_than, BinOpType::b_greater_than},
      {TokenType::t_lesser_equal, BinOpType::b_lesser_equal},
      {TokenType::t_greater_equal, BinOpType::b_greater_equal},

      {TokenType::t_ampersand, BinOpType::b_ampersand},
      {TokenType::t_caret, BinOpType::b_caret},
      {TokenType::t_pipe, BinOpType::b_pipe},
  };

  return binOpMap[tok];
}
}; // namespace rgl
