#pragma once
#include "parser/ast/ASTNode.hpp"

namespace rgl {
class ValueBase;
using ValuePtr = std::shared_ptr<ValueBase>;

class StatementNode : public ASTNode {
public:
  virtual ValuePtr genCode();
};

using Statement = std::unique_ptr<StatementNode>;
}; // namespace rgl
