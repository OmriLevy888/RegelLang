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

static std::unordered_map<TokenType, std::pair<BinOpType, uint8_t>> binOpMap{
    {TokenType::t_dot, {BinOpType::b_dot, 0}},

    {TokenType::t_asterisk, {BinOpType::b_asterisk, 1}},
    {TokenType::t_forward_slash, {BinOpType::b_forward_slash, 1}},
    {TokenType::t_percent, {BinOpType::b_percent, 1}},

    {TokenType::t_plus, {BinOpType::b_plus, 2}},
    {TokenType::t_minus, {BinOpType::b_minus, 2}},

    {TokenType::t_shift_left, {BinOpType::b_shift_left, 3}},
    {TokenType::t_shift_right, {BinOpType::b_shift_right, 3}},

    {TokenType::t_equal_equal, {BinOpType::b_equal_equal, 4}},
    {TokenType::t_not_equal, {BinOpType::b_not_equal, 4}},
    {TokenType::t_lesser_than, {BinOpType::b_lesser_than, 4}},
    {TokenType::t_greater_than, {BinOpType::b_greater_than, 4}},
    {TokenType::t_lesser_equal, {BinOpType::b_lesser_equal, 4}},
    {TokenType::t_greater_equal, {BinOpType::b_greater_equal, 4}},

    {TokenType::t_ampersand, {BinOpType::b_ampersand, 5}},
    {TokenType::t_caret, {BinOpType::b_caret, 6}},
    {TokenType::t_pipe, {BinOpType::b_pipe, 7}},

    {TokenType::t_asterisk_equal, {BinOpType::b_asterisk_equal, 8}},
    {TokenType::t_forward_slash_equal, {BinOpType::b_forward_slash_equal, 8}},
    {TokenType::t_percent_equal, {BinOpType::b_percent_equal, 8}},
    {TokenType::t_plus_equal, {BinOpType::b_plus_equal, 8}},
    {TokenType::t_minus_equal, {BinOpType::b_minus_equal, 8}},
    {TokenType::t_shift_left_equal, {BinOpType::b_shift_left_equal, 8}},
    {TokenType::t_shift_right_equal, {BinOpType::b_shift_right_equal, 8}},
    {TokenType::t_ampersand_equal, {BinOpType::b_ampersand_equal, 8}},
    {TokenType::t_caret_equal, {BinOpType::b_caret_equal, 8}},
};
bool ParserUtilities::isBinOp(const Token &tok) {
  return BinOpType::b_err != tokToBinOpType(tok);
}
uint8_t ParserUtilities::tokToPrecedence(const Token &tok) {
  return binOpMap[tok].second;
}
BinOpType ParserUtilities::tokToBinOpType(const Token &tok) {
  return binOpMap[tok].first;
}
}; // namespace rgl
