#include "lexer/Token.hpp"

namespace rgl {
const std::string &Token::getTypeString() const {
  static std::map<TokenType, std::string> token_types_repr{
      {TokenType::t_err, "t_err"},
      {TokenType::t_eof, "t_eof"},

      {TokenType::t_semicolon, "t_semicolon"},
      {TokenType::t_underscore, "t_underscore"},
      {TokenType::t_comma, "t_comma"},
      {TokenType::t_colon, "t_colon"},
      {TokenType::t_pound, "t_pound"},
      {TokenType::t_open_bracket, "t_open_bracket"},
      {TokenType::t_close_bracket, "t_close_bracket"},
      {TokenType::t_open_paren, "t_open_paren"},
      {TokenType::t_close_paren, "t_close_paren"},
      {TokenType::t_open_square, "t_open_square"},
      {TokenType::t_close_square, "t_close_square"},
      {TokenType::t_dot, "t_dot"},
      {TokenType::t_question_mark, "t_question_mark"},
      {TokenType::t_arrow, "t_arrow"},
      {TokenType::t_at, "t_at"},

      {TokenType::t_plus, "t_plus"},
      {TokenType::t_minus, "t_minus"},
      {TokenType::t_asterisk, "t_asterisk"},
      {TokenType::t_forward_slash, "t_forward_slash"},
      {TokenType::t_percent, "t_percent"},

      {TokenType::t_plus_plus, "t_plus_plus"},
      {TokenType::t_minus_minus, "t_minus_minus"},

      {TokenType::t_exclamation, "t_exclamation"},
      {TokenType::t_caret, "t_caret"},
      {TokenType::t_pipe, "t_pipe"},
      {TokenType::t_ampersand, "t_ampersand"},

      {TokenType::t_shift_left, "t_shift_left"},
      {TokenType::t_shift_right, "t_shift_right"},

      {TokenType::t_equal, "t_equal"},
      {TokenType::t_greater_than, "t_greater_than"},
      {TokenType::t_lesser_than, "t_lesser_than"},
      {TokenType::t_equal_equal, "t_equal_equal"},
      {TokenType::t_not_equal, "t_not_equal"},
      {TokenType::t_greater_equal, "t_greater_equal"},
      {TokenType::t_lesser_equal, "t_lesser_equal"},

      {TokenType::t_plus_equal, "t_plus_equal"},
      {TokenType::t_minus_equal, "t_minus_equal"},
      {TokenType::t_asterisk_equal, "t_asterisk_equal"},
      {TokenType::t_forward_slash_equal, "t_forward_slash_equal"},
      {TokenType::t_percent_equal, "t_percent_equal"},
      {TokenType::t_caret_equal, "t_caret_equal"},
      {TokenType::t_pipe_equal, "t_pipe_equal"},
      {TokenType::t_ampersand_equal, "t_ampersand_equal"},
      {TokenType::t_shift_left_equal, "t_shift_left_equal"},
      {TokenType::t_shift_right_equal, "t_shift_right_equal"},

      {TokenType::t_question_equal, "t_question_equal"},
      {TokenType::t_question_greater_than, "t_question_greater_than"},
      {TokenType::t_question_lesser_than, "t_question_lesser_than"},
      {TokenType::t_question_equal_equal, "t_question_equal_equal"},
      {TokenType::t_question_not_equal, "t_question_not_equal"},
      {TokenType::t_question_greater_equal, "t_question_greater_equal"},
      {TokenType::t_question_lesser_equal, "t_question_lesser_equal"},

      {TokenType::t_in, "t_in"},

      {TokenType::t_boolean, "t_boolean"},

      {TokenType::t_double_literal, "t_double_literal"},
      {TokenType::t_float_literal, "t_float_literal"},

      {TokenType::t_int64_literal, "t_int64_literal"},
      {TokenType::t_int32_literal, "t_int32_literal"},
      {TokenType::t_int16_literal, "t_int16_literal"},
      {TokenType::t_int8_literal, "t_int8_literal"},

      {TokenType::t_uint64_literal, "t_uint64_literal"},
      {TokenType::t_uint32_literal, "t_uint32_literal"},
      {TokenType::t_uint16_literal, "t_uint16_literal"},
      {TokenType::t_uint8_literal, "t_uint8_literal"},

      {TokenType::t_char_literal, "t_char_literal"},
      {TokenType::t_string_literal, "t_string_literal"},

      {TokenType::t_identifier, "t_identifier"},

      {TokenType::t_return, "t_return"},
      {TokenType::t_yield, "t_yield"},

      {TokenType::t_var, "t_var"},
      {TokenType::t_as, "t_as"},

      {TokenType::t_if, "t_if"},
      {TokenType::t_elif, "t_elif"},
      {TokenType::t_else, "t_else"},

      {TokenType::t_for, "t_for"},
      {TokenType::t_while, "t_while"},
      {TokenType::t_times, "t_times"},

      {TokenType::t_break, "t_break"},
      {TokenType::t_continue, "t_continue"},

      {TokenType::t_switch, "t_switch"},

      {TokenType::t_func, "t_func"},

      {TokenType::t_class, "t_class"},
      {TokenType::t_ctor, "t_ctor"},
      {TokenType::t_dtor, "t_dtor"},
      {TokenType::t_move, "t_move"},
      {TokenType::t_pub, "t_pub"},
      {TokenType::t_hook, "t_hook"},

      {TokenType::t_namespace, "t_namespace"},
      {TokenType::t_import, "t_import"}};
  static std::string unknown_repr = "t_repr";

  const auto &ret = token_types_repr[m_tokenType];
  if (ret.length() == 0) {
    return unknown_repr;
  }
  return ret;
}
}; // namespace rgl
