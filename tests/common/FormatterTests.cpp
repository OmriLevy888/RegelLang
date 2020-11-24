#include "tests/TestsCore.hpp"

#include "common/Formatter.hpp"

TEST_CASE("Formatter tests", "[formatter]") {
  REQUIRE(rgl::Formatter("{1} + {2} = {}", 3, 1, 2).toString() == "1 + 2 = 3");
}
