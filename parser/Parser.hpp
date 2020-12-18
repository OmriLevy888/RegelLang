#pragma once
#include "common/Core.hpp"
#include "common/ILoggable.hpp"
#include "lexer/ITokenGenerator.hpp"
#include "parser/StatementParser.hpp"
#include "parser/ast/constructs/FileNode.hpp"
#include <memory>

namespace rgl {
class Parser : public ILoggable {
public:
  Parser(const std::string &path);

  std::unique_ptr<FileNode> parseFile();

  std::string toString() const { return ""; }

private:
  std::shared_ptr<ITokenGenerator> m_tokenGenerator;
  std::unique_ptr<StatementParser> m_statementParser;
};
}; // namespace rgl
