#include "tests/parser/ParserTestsUtilities.hpp"
#include "lexer/DummyTokenGenerator.hpp"
#include "gtest/gtest.h"

namespace rgl {

std::unique_ptr<Parser> makeParser(std::vector<TokenValuePair> &&tokens) {
  auto tokenGenerator =
      std::make_unique<DummyTokenGenerator>(std::move(tokens));
  auto tokenCollection =
      std::make_unique<TokenCollection>(std::move(tokenGenerator));
  return std::make_unique<Parser>(std::move(tokenCollection));
}

void assertNode(std::unique_ptr<ASTNode> expr,
                std::unique_ptr<ASTNode> expected) {
  ASSERT_TRUE(expr != nullptr);
  std::cout << "===========" << std::endl;
  std::cout << expected->toString() << std::endl;
  std::cout << expr->toString() << std::endl;
  ASSERT_TRUE(expr->toString() == expected->toString());
}
}; // namespace rgl
