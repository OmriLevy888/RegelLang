#include "tests/parser/ParserTestsUtilities.hpp"

#include "lexer/DummyTokenGenerator.hpp"
#include "gtest/gtest.h"

namespace rgl {

std::unique_ptr<Parser>
makeParser(std::vector<TokenValuePair> &&tokens,
           std::shared_ptr<SourceProject> sourceProject) {
  auto tokenGenerator =
      std::make_unique<DummyTokenGenerator>(std::move(tokens), sourceProject);
  auto tokenCollection =
      std::make_unique<TokenCollection>(std::move(tokenGenerator));
  return std::make_unique<Parser>(std::move(tokenCollection));
}

std::unique_ptr<Parser> makeParser(const std::string &testName,
                                   std::vector<TokenValuePair> &&tokens,
                                   std::vector<std::string> &&source,
                                   const size_t firstLineNo) {
  auto project = std::make_shared<SourceProject>(testName);

  SourceFile file{testName};
  file.m_lines.reserve(source.size());
  for (size_t idx = 0; idx < source.size(); idx++) {
    file.m_lines.emplace_back(std::move(source[idx]), tokens,
                              firstLineNo + idx);
  }

  project->addFile(std::move(file));
  return std::move(makeParser(std::move(tokens), project));
}

void assertNode(std::unique_ptr<ASTNode> expr,
                std::unique_ptr<ASTNode> expected) {
  ASSERT_TRUE(expr != nullptr);
  std::cout << "===========" << std::endl;
  std::cout << expected->toString() << std::endl;
  std::cout << expr->toString() << std::endl;
  ASSERT_EQ(expr->toString(), expected->toString());
}
}; // namespace rgl
