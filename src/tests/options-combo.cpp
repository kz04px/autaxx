#include <catch2/catch.hpp>
#include "../autaxx/options.hpp"

TEST_CASE("Options::combos()") {
    Options::combos["TestCombo"] = Options::Combo("A", {"A", "B", "C"});
    REQUIRE(Options::combos["TestCombo"].get() == "A");
    Options::set("TestCombo", "B");
    REQUIRE(Options::combos["TestCombo"].get() == "B");
    Options::set("TestCombo", "C");
    REQUIRE(Options::combos["TestCombo"].get() == "C");
    Options::set("TestCombo", "D");
    REQUIRE(Options::combos["TestCombo"].get() == "C");
}
