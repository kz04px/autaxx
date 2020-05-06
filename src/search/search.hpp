#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <cstdint>
#include <cstring>
#include <libataxx/position.hpp>
#include <memory>
#include <thread>

namespace search {

enum Type
{
    Time = 0,
    Depth,
    Nodes,
    Movetime,
    Infinite
};

struct Settings {
    int type = Type::Time;
    // Time search
    int btime = -1;
    int wtime = -1;
    int binc = -1;
    int winc = -1;
    int movestogo = -1;
    // Movetime search
    int movetime = -1;
    // Nodes search
    std::uint64_t nodes = -1;
    // Depth search
    int depth = -1;
};

struct Stats {
    void clear() {
        nodes = 0;
        tthits = 0;
        seldepth = 0;
#ifndef NDEBUG
        std::memset(cutoffs, 0, libataxx::max_moves * sizeof(std::uint64_t));
#endif
    }
    std::uint64_t nodes = 0;
    std::uint64_t tthits = 0;
    int seldepth = 0;
#ifndef NDEBUG
    std::uint64_t cutoffs[libataxx::max_moves] = {};
#endif
};

struct Controller {
    std::uint64_t max_nodes = 0;
    std::chrono::high_resolution_clock::time_point end_time;
    volatile bool stop = false;
};

class Search {
   public:
    virtual ~Search() {
        stop();
    }

    virtual void go(const libataxx::Position pos, const Settings &settings) = 0;

    void stop() noexcept {
        controller_.stop = true;
        if (search_thread_.joinable()) {
            search_thread_.join();
        }
        controller_.stop = false;
    }

    virtual void clear() noexcept {
    }

   protected:
    std::thread search_thread_;
    Stats stats_;
    Controller controller_;
};

extern std::unique_ptr<Search> search_main;

}  // namespace search

#endif
