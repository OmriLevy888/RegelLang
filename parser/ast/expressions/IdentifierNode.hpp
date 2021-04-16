#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class IdentifierNode : public ExpressionNode {};

using Identifier = std::unique_ptr<IdentifierNode>;
}; // namespace rgl
