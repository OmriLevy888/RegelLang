#pragma once
#include "parser/ast/ASTNode.hpp"

namespace rgl {
class StatementNode : public ASTNode {};

using Statement = std::unique_ptr<StatementNode>;
}; // namespace rgl
