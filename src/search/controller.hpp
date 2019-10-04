#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <chrono>
#include <cstdint>

namespace search {

struct Controller {
    volatile bool *stop;
    std::uint64_t max_nodes;
    std::chrono::high_resolution_clock::time_point end_time;
};

}  // namespace search

#endif
