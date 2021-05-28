#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"
#include <vector>

namespace rgl {
class BlockNode : public ExpressionNode {
public:
  BlockNode() : m_statements() {}
  BlockNode(std::vector<Statement> &&statements)
      : m_statements(std::move(statements)) {}

  virtual llvm::Value *genCode() override;

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string inner = "";
    if (0 != m_statements.size()) {
      std::string spacesStr(spaces + 17, ' ');
      std::string statementsStr = "";
      auto it = m_statements.cbegin();
      for (; it + 1 != m_statements.cend(); it++) {
        statementsStr +=
            Formatter("{},\n{}", (*it)->toTreeStr(spaces + 17), spacesStr);
      }
      statementsStr += (*it)->toTreeStr(spaces + 17);
      inner = Formatter("statements:{}", statementsStr);
    }
    return Formatter("Block<{}>", inner);
  }

private:
  std::vector<Statement> m_statements;
};

using Block = std::unique_ptr<BlockNode>;
}; // namespace rgl

// TODO:
// Change name to ScopeNode.
// Each scope contains a list of defiend types, functions and statements.
// Codegen for a scope starts by defining all the types, then defining all the
// functions and lastly generating code for all the functions and statements.

// TODO:
// Implement `undefined` keyword so that when a variable is declared and has no
// initial value, this keyword has to be used to implicitly denote that the
// variable is uninitialized.
