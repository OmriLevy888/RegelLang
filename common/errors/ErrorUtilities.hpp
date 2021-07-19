#pragma once
#include <memory>

namespace rgl {
class Token;
class SourceProject;
class TokenCollection;

std::string tokenToString(Token tok);
std::string tokenToString(const std::unique_ptr<TokenCollection> &tokens);

std::string pointAt(Token tok);
std::string pointAt(const std::unique_ptr<TokenCollection> &tokens);
}; // namespace rgl
