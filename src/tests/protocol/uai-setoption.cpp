#include <catch2/catch.hpp>
#include "../../autaxx/options.hpp"
#include "../../autaxx/protocol/uai/setoption.hpp"

TEST_CASE("Options::spin()") {
    Options::strings["Test Name"] = Options::String("Replace");
    std::stringstream ss("name Test Name value Some Value");
    UAI::setoption(ss);
    REQUIRE(Options::strings["Test Name"].get() == "Some Value");
}
