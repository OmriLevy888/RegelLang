#pragma once
#include "parser/ast/statements/StatementNode.hpp"

namespace rgl {
class ContinueNode : public StatementNode {
public:
  virtual std::string toTreeStr(size_t spaces) const override {
    return "Continue<>";
  }
};
}; // namespace rgl
