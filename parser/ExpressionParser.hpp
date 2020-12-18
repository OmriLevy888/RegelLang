#pragma once
#include "common/ILoggable.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include <memory>

namespace rgl {
class ExpressionParser : public ILoggable {
public:
  ExpressionParser(std::shared_ptr<ITokenGenerator>);

  std::unique_ptr<ExpressionNode> parseExpression();

  std::string toString() const { return ""; }

private:
  std::shared_ptr<ITokenGenerator> m_tokenGenerator;
};
}; // namespace rgl
