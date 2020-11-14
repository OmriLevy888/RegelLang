#include "lexer/Lexer.h"

namespace rgl {
Token Lexer::getNext() {
    return Token(TokenType::t_eof);
}
}