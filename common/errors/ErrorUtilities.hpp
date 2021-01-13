#pragma once
#include "common/collections/source-objects/SourceProject.hpp"
#include "lexer/Token.hpp"
#include "lexer/TokenCollection.hpp"

namespace rgl {
std::string tokenToString(Token tok,
                          std::shared_ptr<SourceProject> sourceProject);
std::string tokenToString(const std::unique_ptr<TokenCollection> &tokens);

std::string pointAt(Token tok, std::shared_ptr<SourceProject> sourceProject);
std::string pointAt(const std::unique_ptr<TokenCollection> &tokens);
}; // namespace rgl
