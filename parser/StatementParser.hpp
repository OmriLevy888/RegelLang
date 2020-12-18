#pragma once
#include "lexer/ITokenGenerator.hpp"
#include "parser/ExpressionParser.hpp"
#include "parser/ast/statement/StatementNode.hpp"
#include <memory>

namespace rgl {
class StatementParser {
public:
  StatementParser(std::shared_ptr<ITokenGenerator>);

  std::unique_ptr<StatementNode> parseStatement();

  std::string toString() const { return ""; }

private:
  std::shared_ptr<ITokenGenerator> m_tokenGenerator;
  std::unique_ptr<ExpressionParser> m_expressionParser;
};
}; // namespace rgl
