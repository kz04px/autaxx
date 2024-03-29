#include <catch2/catch.hpp>
#include "../autaxx/options.hpp"

TEST_CASE("Options::spins()") {
    Options::spins["TestSpin"] = Options::Spin(0, 10, 5);
    REQUIRE(Options::spins["TestSpin"].get() == 5);
    Options::set("TestSpin", "-2");
    REQUIRE(Options::spins["TestSpin"].get() == 0);
    Options::set("TestSpin", "12");
    REQUIRE(Options::spins["TestSpin"].get() == 10);
}
