#pragma once
#include <map>
#include <memory>
#include <optional>
#include <string_view>
#include <variant>

#include "common/Formatter.hpp"
#include "common/source-objects/SourceLocation.hpp"

namespace rgl {
enum class TokenType : uint8_t {
  t_err,
  t_eof,

  t_semicolon,     // ;
  t_underscore,    // _
  t_comma,         // ,
  t_colon,         // :
  t_pound,         // #
  t_open_bracket,  // {
  t_close_bracket, // }
  t_open_paren,    // (
  t_close_paren,   // )
  t_open_square,   // [
  t_close_square,  // ]
  t_dot,           // .
  t_question_mark, // ?
  t_arrow,         // =>
  t_at,            // @

  t_plus,          // +
  t_minus,         // -
  t_asterisk,      // *
  t_forward_slash, // /
  t_percent,       // %

  t_plus_plus,   // ++
  t_minus_minus, // --

  t_exclamation, // !
  t_caret,       // ^
  t_pipe,        // |
  t_ampersand,   // &

  t_shift_left,  // <<
  t_shift_right, // >>

  t_equal,         // =
  t_greater_than,  // >
  t_lesser_than,   // <
  t_equal_equal,   // ==
  t_not_equal,     // !=
  t_greater_equal, // >=
  t_lesser_equal,  // <=

  t_plus_equal,          // +=
  t_minus_equal,         // -=
  t_asterisk_equal,      // *=
  t_forward_slash_equal, // /=
  t_percent_equal,       // %=
  t_caret_equal,         // ^=
  t_pipe_equal,          // |=
  t_ampersand_equal,     // &=
  t_shift_left_equal,    // <<=
  t_shift_right_equal,   // >>=

  t_question_equal,         // ?=
  t_question_greater_than,  // ?>
  t_question_lesser_than,   // ?<
  t_question_equal_equal,   // ?==
  t_question_not_equal,     // ?!=
  t_question_greater_equal, // ?>=
  t_question_lesser_equal,  // ?<=

  t_in, // in

  t_boolean,

  t_double_literal,
  t_float_literal,

  t_int64_literal,
  t_int32_literal,
  t_int16_literal,
  t_int8_literal,

  t_uint64_literal,
  t_uint32_literal,
  t_uint16_literal,
  t_uint8_literal,

  t_char_literal,
  t_string_literal, // TODO: Add all string literal types

  t_identifier,

  t_return, // return
  t_yield,  // yield

  t_var, // var
  t_as,  // as

  t_if,   // if
  t_elif, // elif
  t_else, // else

  t_for,      // for
  t_while,    // while
  t_times,    // times
  t_break,    // break
  t_continue, // continue

  t_switch, // switch

  t_func, // func

  t_class, // class
  t_ctor,  // ctor
  t_dtor,  // dtor
  t_move,  // move
  t_pub,   // pub
  t_hook,  // hook

  t_namespace, // namespace
  t_import,    // import
};

class Token : public ILoggable {
public:
  Token(TokenType type = TokenType::t_eof, SourceLocation sourceLocation = {})
      : m_sourceLocation(sourceLocation), m_tokenType(type) {}

  inline SourceLocation &sourceLocation() noexcept { return m_sourceLocation; }
  inline SourceLocation sourceLocation() const noexcept {
    return m_sourceLocation;
  }

  inline bool isEOF() const noexcept { return m_tokenType == TokenType::t_eof; }

  std::string toString() const override {
    return Formatter("Token<{}>", getTypeString()).toString();
  }

  operator TokenType() const { return m_tokenType; }

private:
  SourceLocation m_sourceLocation;
  TokenType m_tokenType;

  const std::string &getTypeString() const;
};

using TokenValue =
    std::variant<bool, char, int64_t, uint64_t, float, double, std::string>;

class TokenValuePair {
public:
  Token m_token;
  std::optional<TokenValue> m_value;

  TokenValuePair(Token &&token) : m_token(token), m_value(std::nullopt) {}
  TokenValuePair(Token &&token, std::optional<TokenValue> &&value)
      : m_token(token), m_value(value) {}
  TokenValuePair(const Token &token) : m_token(token), m_value(std::nullopt) {}
  TokenValuePair(const Token &token, std::optional<TokenValue> &&value)
      : m_token(token), m_value(value) {}
  TokenValuePair(const Token &token, const std::optional<TokenValue> &value)
      : m_token(token), m_value(value) {}

  operator TokenType() const { return m_token; }
};
} // namespace rgl
