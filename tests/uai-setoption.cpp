#include <catch.hpp>
#include "../src/options.hpp"
#include "../src/protocol.hpp"

TEST_CASE("Options::spin()") {
    Options::strings["Test Name"] = Options::String("Replace");
    std::stringstream ss("name Test Name value Some Value");
    UAI::setoption(ss);
    REQUIRE(Options::strings["Test Name"].get() == "Some Value");
}
