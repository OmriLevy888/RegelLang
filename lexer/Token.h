#pragma once
#include "common/Core.h"

#include <string_view>
#include <map>

namespace rgl {
enum class TokenType : uint16_t {
    t_eof,

    t_semicolon,  // ;
    t_underscore,  // _
    t_comma,  // ,
    t_colon,  // :
    t_pound,  // #
    t_open_bracket,  // {
    t_close_bracket,  // }
    t_open_paren,  // (
    t_close_paren,  // )
    t_open_square,  // [
    t_close_square,  // ]
    t_dot_dot,  // ..
    t_question_mark,  // ?

    t_plus,  // +
    t_minus,  // -
    t_asterisk,  // *
    t_forward_slash,  // /

    t_equal,  // =
    t_greater_than,  // >
    t_lesser_than,  // <
    t_equal_equal,  // ==
    t_not_equal,  // !=
    t_greater_equal,  // >=
    t_lesser_equal,  // <=

    t_question_equal,  // ?=
    t_question_greater_than,  // ?>
    t_question_lesser_than,  // ?<
    t_question_equal_equal,  // ?==
    t_question_not_equal,  // ?!=
    t_question_greater_equal,  // ?>=
    t_question_lesser_equal,  // ?<=

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
    t_string_literal,  // TODO: Add all string literal types

    t_identifier,

    t_return,  // return
    t_yield,  // yield

    t_let,  // let
    t_var,  // var
    t_as,  // as

    t_if,  // if
    t_elif,  // elif
    t_else,  // else

    t_for,  // for
    t_while,  // while
    t_times,  // times
    t_break,  // break
    t_continue,  // continue

    t_switch,  // switch

    t_func,  // func
    t_ret_type,  // =>

    t_class,  // class
    t_ctor,  // ctor
    t_dtor,  // dtor
    t_move,  // move
};

class Token : public ILoggable {
public:
    Token(TokenType type=TokenType::t_eof, uint32_t reprStartIdx=0, uint16_t reprLen=0, 
        uint16_t fileNo=0, uint32_t lineNo=0, uint16_t tokenIdx=0)
    : m_tokenIdx(tokenIdx), m_reprLen(reprLen)
    {
        setTokenType(type);
        setFileNo(fileNo);
        setLineNo(lineNo);
        setReprStartIdx(reprStartIdx);
    }

    inline bool isEOF() const noexcept { return getTokenType() == TokenType::t_eof; }

    std::string toString() const override {
        return "Token<>";
    }

    void setLineNo(uint32_t val) noexcept { 
        m_meta1 &= 0xfff;
        m_meta1 |= val << 12;
    }
    void setReprStartIdx(uint32_t val) noexcept {
        m_meta1 &= 0xfffff000;
        m_meta1 |= (val & 0xfff00) >> 8;
        m_meta2 &= 0x00ffffff;
        m_meta2 |= (val & 0xff) << 24;
    }
    void setFileNo(uint16_t val) noexcept {
        m_meta2 &= 0xff0003ff;
        m_meta2 |= (val & 0x3fff) << 10;
    }
    void setTokenType(TokenType val) noexcept {
        m_meta2 &= 0xfffffc00;
        m_meta2 |= static_cast<uint16_t>(val) & 0x3ff;
    }
    void setTokenIdx(uint16_t val) noexcept { 
        m_tokenIdx = val; 
    }
    void setReprLen(uint16_t val) noexcept { 
        m_reprLen = val; 
    } 

    uint32_t getLineNo() const noexcept { 
        return (m_meta1 >> 12) & 0xfffff; 
    }
    uint32_t getReprStartIdx() const noexcept {
        return ((m_meta1 & 0xfff) << 8) | ((m_meta2 >> 24) & 0xff);
    }
    uint16_t getFileNo() const noexcept {
        return (m_meta2 >> 10) & 0x3fff; 
    }
    TokenType getTokenType() const noexcept {
        return static_cast<TokenType>(m_meta2 & 0x3ff); 
    }
    uint16_t getTokenIdx() const noexcept { 
        return m_tokenIdx; 
    }
    uint16_t getReprLen() const noexcept { 
        return m_reprLen; 
    }

private:
    // 20 bits line no.
    // 20 bits repr start idx
    // 14 bits file no.
    // 10 bits token type
    // 16 bits token idx
    // 16 bits repr len
    uint32_t m_meta1;
    uint32_t m_meta2;
    uint16_t m_tokenIdx;
    uint16_t m_reprLen;

    const std::string &token_type_to_string() const {
        static std::map<TokenType, std::string> token_types_repr{
            {TokenType::t_eof,                          "t_eof"},

            {TokenType::t_semicolon,                    "t_semicolon"},
            {TokenType::t_underscore,                   "t_underscore"},
            {TokenType::t_comma,                       "t_comma"},
            {TokenType::t_colon,                        "t_colon"},
            {TokenType::t_pound,                        "t_pound"},
            {TokenType::t_open_bracket,                 "t_open_bracket"},
            {TokenType::t_close_bracket,                "t_close_bracket"},
            {TokenType::t_open_paren,                   "t_open_paren"},
            {TokenType::t_close_paren,                  "t_close_paren"},
            {TokenType::t_open_square,                  "t_open_square"},
            {TokenType::t_close_square,                 "t_close_square"},
            {TokenType::t_dot_dot,                      "t_dot_dot"},
            {TokenType::t_question_mark,                "t_question_mark"},

            {TokenType::t_plus,                         "t_plus"},
            {TokenType::t_minus,                        "t_minus"},
            {TokenType::t_asterisk,                     "t_asterisk"},
            {TokenType::t_forward_slash,                "t_forward_slash"},

            {TokenType::t_equal,                        "t_equal"},
            {TokenType::t_greater_than,                 "t_greater_than"},
            {TokenType::t_lesser_than,                  "t_lesser_than"},
            {TokenType::t_equal_equal,                  "t_equal_equal"},
            {TokenType::t_not_equal,                    "t_not_equal"},
            {TokenType::t_greater_equal,                "t_greater_equal"},
            {TokenType::t_lesser_equal,                 "t_lesser_equal"},

            {TokenType::t_question_equal,               "t_question_equal"},
            {TokenType::t_question_greater_than,        "t_question_greater_than"},
            {TokenType::t_question_lesser_than,         "t_question_lesser_than"},
            {TokenType::t_question_equal_equal,         "t_question_equal_equal"},
            {TokenType::t_question_not_equal,           "t_question_not_equal"},
            {TokenType::t_question_greater_equal,       "t_question_greater_equal"},
            {TokenType::t_question_lesser_equal,        "t_question_lesser_equal"},

            {TokenType::t_double_literal,               "t_double_literal"},
            {TokenType::t_float_literal,                "t_float_literal"},

            {TokenType::t_int64_literal,                "t_int64_literal"},
            {TokenType::t_int32_literal,                "t_int32_literal"},
            {TokenType::t_int16_literal,                "t_int16_literal"},
            {TokenType::t_int8_literal,                 "t_int8_literal"},

            {TokenType::t_uint64_literal,               "t_uint64_literal"},
            {TokenType::t_uint32_literal,               "t_uint32_literal"},
            {TokenType::t_uint16_literal,               "t_uint16_literal"},
            {TokenType::t_uint8_literal,                "t_uint8_literal"},

            {TokenType::t_char_literal,                 "t_char_literal"},
            {TokenType::t_string_literal,               "t_string_literal"},

            {TokenType::t_identifier,                   "t_identifier"},

            {TokenType::t_return,                       "t_return"},
            {TokenType::t_yield,                        "t_yield"},

            {TokenType::t_let,                          "t_let"},
            {TokenType::t_var,                          "t_var"},
            {TokenType::t_as,                           "t_as"},

            {TokenType::t_if,                           "t_if"},
            {TokenType::t_elif,                         "t_elif"},
            {TokenType::t_else,                         "t_else"},

            {TokenType::t_for,                          "t_for"},
            {TokenType::t_while,                        "t_while"},
            {TokenType::t_times,                        "t_times"},

            {TokenType::t_break,                        "t_break"},
            {TokenType::t_continue,                     "t_continue"},

            {TokenType::t_switch,                       "t_switch"},

            {TokenType::t_func,                         "t_func"},
            {TokenType::t_ret_type,                     "t_ret_type"},

            {TokenType::t_class,                        "t_class"},
            {TokenType::t_ctor,                         "t_ctor"},
            {TokenType::t_dtor,                         "t_dtor"},
            {TokenType::t_move,                         "t_move"}
        };
        static std::string unknown_repr = "t_repr";

        const auto ret = token_types_repr[getTokenType()];
        if (ret.length() == 0) {
            return unknown_repr;
        }
        return ret;
    }
};
}  // namespace rgl