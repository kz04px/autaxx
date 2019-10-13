#include <catch.hpp>
#include <climits>
#include <cstdint>
#include "../src/search/tt.hpp"
#include "../src/utils.hpp"

struct Entry {
    std::uint64_t hash;
    std::uint32_t n;
};

TEST_CASE("hashtable") {
    search::TT<Entry> tt{16};

    for (int i = 0; i < 512; ++i) {
        const std::uint64_t hash =
            utils::rand_u64(0, std::numeric_limits<std::uint64_t>::max());
        const std::uint32_t n =
            utils::rand_u32(0, std::numeric_limits<std::uint32_t>::max());

        tt.add(hash, Entry{hash, n});
        const auto entry = tt.poll(hash);

        REQUIRE(entry.hash == hash);
        REQUIRE(entry.n == n);
    }
}
