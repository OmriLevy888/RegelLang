#include "parser/ParserUtilities.hpp"
#include "lexer/Token.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"

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

    {TokenType::t_asterisk, {BinOpType::b_asterisk, 2}},
    {TokenType::t_forward_slash, {BinOpType::b_forward_slash, 2}},
    {TokenType::t_percent, {BinOpType::b_percent, 2}},

    {TokenType::t_plus, {BinOpType::b_plus, 3}},
    {TokenType::t_minus, {BinOpType::b_minus, 3}},

    {TokenType::t_shift_left, {BinOpType::b_shift_left, 4}},
    {TokenType::t_shift_right, {BinOpType::b_shift_right, 4}},

    {TokenType::t_equal_equal, {BinOpType::b_equal_equal, 5}},
    {TokenType::t_not_equal, {BinOpType::b_not_equal, 5}},
    {TokenType::t_lesser_than, {BinOpType::b_lesser_than, 5}},
    {TokenType::t_greater_than, {BinOpType::b_greater_than, 5}},
    {TokenType::t_lesser_equal, {BinOpType::b_lesser_equal, 5}},
    {TokenType::t_greater_equal, {BinOpType::b_greater_equal, 5}},

    {TokenType::t_ampersand, {BinOpType::b_ampersand, 6}},
    {TokenType::t_caret, {BinOpType::b_caret, 7}},
    {TokenType::t_pipe, {BinOpType::b_pipe, 8}},

    {TokenType::t_asterisk_equal, {BinOpType::b_asterisk_equal, 9}},
    {TokenType::t_forward_slash_equal, {BinOpType::b_forward_slash_equal, 9}},
    {TokenType::t_percent_equal, {BinOpType::b_percent_equal, 9}},
    {TokenType::t_plus_equal, {BinOpType::b_plus_equal, 9}},
    {TokenType::t_minus_equal, {BinOpType::b_minus_equal, 9}},
    {TokenType::t_shift_left_equal, {BinOpType::b_shift_left_equal, 9}},
    {TokenType::t_shift_right_equal, {BinOpType::b_shift_right_equal, 9}},
    {TokenType::t_ampersand_equal, {BinOpType::b_ampersand_equal, 9}},
    {TokenType::t_caret_equal, {BinOpType::b_caret_equal, 9}},
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

bool ParserUtilities::isUnaryOp(const Token &tok) {
  return isPreOp(tok) || isPostOp(tok);
}
bool ParserUtilities::isPreOp(const Token &tok) {
  switch (tok) {
  case TokenType::t_plus_plus:
  case TokenType::t_minus_minus:
  case TokenType::t_exclamation:
    return true;
  default:
    return false;
  }
}
bool ParserUtilities::isPostOp(const Token &tok) {
  switch (tok) {
  case TokenType::t_plus_plus:
  case TokenType::t_minus_minus:
    return true;
  default:
    return false;
  }
}
std::unordered_map<TokenType, UnaryOpType> preOpMap{
    {TokenType::t_plus_plus, UnaryOpType::u_pre_plus_plus},
    {TokenType::t_minus_minus, UnaryOpType::u_pre_minus_miuns},

    {TokenType::t_exclamation, UnaryOpType::u_pre_exclamation},
};
UnaryOpType ParserUtilities::tokToPreOpType(const Token &tok) {
  return preOpMap[tok];
}
std::unordered_map<TokenType, UnaryOpType> postOpMap{
    {TokenType::t_plus_plus, UnaryOpType::u_post_plus_plus},
    {TokenType::t_minus_minus, UnaryOpType::u_post_minus_minus},
};
UnaryOpType ParserUtilities::tokToPostOpType(const Token &tok) {
  return postOpMap[tok];
}

bool ParserUtilities::isVarDecl(const Token &tok) {
  return TokenType::t_let == tok || TokenType::t_var == tok;
}

bool ParserUtilities::isSimpleStatement(const Token &tok) {
  switch (tok) {
  case TokenType::t_return:
  case TokenType::t_yield:
    return true;
  default:
    return false;
  }
}
}; // namespace rgl
