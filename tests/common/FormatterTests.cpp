#include "tests/TestsCore.hpp"

#include "common/Formatter.hpp"
#include <string>

TEST(Formatter, fmt) {
  ASSERT_EQ(rgl::Formatter("{1} + {2} = {}", 3, 1, 2).toString(), "1 + 2 = 3");
}

TEST(Formatter, join) {
  ASSERT_EQ(rgl::Formatter<>::join(' ', "a", "b", 2), "a b 2");
}
