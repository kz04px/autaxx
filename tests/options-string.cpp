#include <catch.hpp>
#include "../src/options.hpp"

TEST_CASE("Options::strings()") {
    Options::strings["TestString"] = Options::String("Test");
    REQUIRE(Options::strings["TestString"].get() == "Test");
    Options::set("TestString", "Test2");
    REQUIRE(Options::strings["TestString"].get() == "Test2");

    // Multiple word name/value
    Options::strings["Test String"] = Options::String("Test Value");
    REQUIRE(Options::strings["Test String"].get() == "Test Value");
    Options::set("Test String", "Test Value2");
    REQUIRE(Options::strings["Test String"].get() == "Test Value2");
}
