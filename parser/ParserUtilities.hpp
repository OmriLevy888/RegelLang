#pragma once
#include "lexer/Token.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"

namespace rgl {
class ParserUtilities {
public:
  static bool isIdentifier(const Token &tok);
  static bool isLiteral(const Token &tok);
  static bool isIntLiteral(const Token &tok);
  static bool isRealLiteral(const Token &tok);
  static bool isTextLiteral(const Token &tok);
  static bool isBooleanLiteral(const Token &tok);

  static bool isSignedIntLiteral(const Token &tok);
  static bool isUnsignedIntLiteral(const Token &tok);

  static bool isBinOp(const Token &tok);
  static uint8_t tokToPrecedence(const Token &tok);
  static BinOpType tokToBinOpType(const Token &tok);
};
}; // namespace rgl
