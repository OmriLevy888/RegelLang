#pragma once
#include "parser/ast/ASTNode.hpp"

namespace rgl {
class StatementNode : public ASTNode {
public:
  virtual void genCode() {}
};

using Statement = std::unique_ptr<StatementNode>;
}; // namespace rgl
