#pragma once
#include "lexer/ITokenGenerator.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/ASTNode.hpp"

namespace rgl {
std::unique_ptr<Parser> makeParser(std::vector<TokenValuePair> &&tokens);
void assertNode(std::unique_ptr<ASTNode> node,
                std::unique_ptr<ASTNode> expected);
}; // namespace rgl
