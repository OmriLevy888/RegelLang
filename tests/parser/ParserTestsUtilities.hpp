#pragma once
#include "common/collections/source-objects/SourceProject.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/ASTNode.hpp"

namespace rgl {
std::unique_ptr<Parser> makeParser(
    std::vector<TokenValuePair> &&tokens,
    std::shared_ptr<SourceProject> sourceProject = nullptr);
void assertNode(std::unique_ptr<ASTNode> node,
                std::unique_ptr<ASTNode> expected);
};  // namespace rgl
