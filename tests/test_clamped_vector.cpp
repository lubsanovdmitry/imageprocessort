#include <catch2/catch_all.hpp>

#include "Utils/ClampedVector.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Clamped Vector works") {
    image_processor::ClampedVector<int> cv(2);

    REQUIRE(cv.Size() == 2);
    REQUIRE(cv[0] == 0);

    cv[0] = 1;
    cv[1] = 2;
    REQUIRE(cv[-1] == 1);
    REQUIRE(cv[2] == 2);

    cv.Erase(cv.begin());

    REQUIRE(cv.Size() == 1 && cv[0] == 2);

    cv.Clear();
}