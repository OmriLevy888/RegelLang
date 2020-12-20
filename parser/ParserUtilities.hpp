#pragma once
#include "lexer/Token.hpp"

namespace rgl {
class ParserUtilities {
public:
  static bool isIdentifier(const Token &tok);
  static bool isLiteral(const Token &tok);
  static bool isIntLiteral(const Token &tok);
  static bool isRealLiteral(const Token &tok);
  static bool isTextLiteral(const Token &tok);
  static bool isBooleanLiteral(const Token &tok);
};
}; // namespace rgl
