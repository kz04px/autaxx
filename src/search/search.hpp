#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <cstdint>
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
        seldepth = 0;
    }
    std::uint64_t nodes = 0;
    int seldepth = 0;
};

struct Controller {
    std::uint64_t max_nodes = 0;
    std::chrono::high_resolution_clock::time_point end_time;
    volatile bool stop = false;
};

class Search {
   public:
    ~Search() {
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

   protected:
    std::thread search_thread_;
    Stats stats_;
    Controller controller_;
};

extern std::unique_ptr<Search> search_main;

}  // namespace search

#endif
