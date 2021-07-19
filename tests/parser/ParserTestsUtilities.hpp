#pragma once
#include "parser/ast/ASTNode.hpp"

namespace rgl {
class Parser;

std::unique_ptr<Parser> makeParser(std::vector<TokenValuePair> &&tokens);

std::unique_ptr<Parser> makeParser(const std::string &testName,
                                   std::vector<TokenValuePair> &&tokens,
                                   std::vector<std::string> &&source,
                                   const size_t firstLineNo = 0);

void assertNode(std::unique_ptr<ASTNode> node,
                std::unique_ptr<ASTNode> expected);
}; // namespace rgl
