#include <catch2/catch.hpp>
#include "../src/options.hpp"

TEST_CASE("Options::checks()") {
    Options::checks["TestCheck"] = Options::Check(false);
    REQUIRE(Options::checks["TestCheck"].get() == false);
    Options::set("TestCheck", "true");
    REQUIRE(Options::checks["TestCheck"].get() == true);
}
