#pragma once
#include "common/Core.hpp"
#include "parser/ast/ASTNode.hpp"
#include "parser/ast/Type.hpp"

namespace rgl {
class ExpressionNode : public ASTNode {
public:
  Type getType();
  // TODO: add getValue function that returns an LLVMValue

private:
  Type m_type;
};
}; // namespace rgl
