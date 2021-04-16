#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
class ImportNode : public ExpressionNode {
public:
  ImportNode(Identifier name) : m_name(std::move(name)) {}

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  Identifier m_name;
};

using Import = std::unique_ptr<ImportNode>;
}; // namespace rgl
