#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"

namespace rgl {
class IdentifierNode : public ExpressionNode {
public:
  virtual std::vector<std::string> get() const = 0;
};

using Identifier = std::unique_ptr<IdentifierNode>;
}; // namespace rgl
