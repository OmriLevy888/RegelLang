#pragma once
#include "parser/ast/expressions/BlockNode.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/statements/StatementNode.hpp"

namespace rgl {
enum class PassType : uint8_t { p_const, p_mutable, p_consume };
static std::string passTypeToStr(PassType type) {
  switch (type) {
  case PassType::p_const:
    return "p_const";
  case PassType::p_mutable:
    return "p_mutable";
  case PassType::p_consume:
    return "p_consume";
  default:
    return "p_err";
  }
}

class ForInLoopNode : public ExpressionNode {
public:
  ForInLoopNode(PassType type, Identifier identifier, Expression iterrable,
                Block body)
      : m_type(type), m_identifier(std::move(identifier)),
        m_iterrable(std::move(iterrable)), m_body(std::move(body)) {}
  ForInLoopNode(PassType type, Identifier identifier, Expression iterrable,
                Statement body)
      : m_type(type), m_identifier(std::move(identifier)),
        m_iterrable(std::move(iterrable)) {
    std::vector<Statement> statements;
    statements.push_back(std::move(body));
    m_body = std::make_unique<BlockNode>(std::move(statements));
  }

  virtual std::string toTreeStr(size_t spaces) const override {
    std::string spacesStr(spaces + 14, ' ');
    return Formatter(
        "ForInLoopNode<type:{},\n{}identifier:{}\n{}iterrable:{},\n{}body:{}>",
        passTypeToStr(m_type), spacesStr, m_identifier->toTreeStr(spaces + 25),
        spacesStr, m_iterrable->toTreeStr(spaces + 24), spacesStr,
        m_body->toTreeStr(spaces + 19));
  }

private:
  PassType m_type;
  Identifier m_identifier;
  Expression m_iterrable;
  Block m_body;
};
}; // namespace rgl
