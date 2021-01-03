#pragma once
#include "lexer/Token.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"

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

  static bool isUnaryOp(const Token &tok);
  static bool isPreOp(const Token &tok);
  static bool isPostOp(const Token &tok);
  static UnaryOpType tokToPreOpType(const Token &tok);
  static UnaryOpType tokToPostOpType(const Token &tok);
  static size_t getPreOpPrecedence() noexcept { return 1; }

  static bool isVarDecl(const Token &tok);

  static bool isSimpleStatement(const Token &tok);
  static bool isImplicityStatementExpression(const Token &tok);
};
}; // namespace rgl
