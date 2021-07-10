#pragma once
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/literals/class-literal/ClassLiteralNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"
#include <vector>

namespace rgl {
class ScopeNode : public ExpressionNode {
public:
  ScopeNode() : m_statements() {}
  ScopeNode(std::vector<ClassPtr> &&classes,
            std::vector<FunctionPtr> &&functions,
            std::vector<Statement> &&statements)
      : m_classes(std::move(classes)), m_functions(std::move(functions)),
        m_statements(std::move(statements)) {}
  ScopeNode(std::vector<Statement> &&statements)
      : m_classes(std::vector<ClassPtr>{}),
        m_functions(std::vector<FunctionPtr>{}),
        m_statements(std::move(statements)) {}

  virtual llvm::Value *genCode() override;

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  std::vector<ClassPtr> m_classes;
  std::vector<FunctionPtr> m_functions;
  std::vector<Statement> m_statements;
};

using Scope = std::unique_ptr<ScopeNode>;
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
