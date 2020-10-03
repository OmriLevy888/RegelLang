#include "tests/TestsCore.h"

#include "common/Formatter.h"

TEST_CASE("Formatter tests", "[formatter]") {
    REQUIRE(rgl::Formatter("{1} + {2} = {}", 3, 1, 2).toString() == "1 + 2 = 3");
}