#ifndef SEARCH_MCTS_HPP
#define SEARCH_MCTS_HPP

#include <libataxx/position.hpp>
#include "../search.hpp"

namespace search {

namespace mcts {

class MCTS : public Search {
   public:
    void go(const libataxx::Position pos, const Settings &settings) override {
        stop();
        search_thread_ = std::thread(&MCTS::root, this, pos, settings);
    }

   private:
    void root(const libataxx::Position pos, const Settings &settings) noexcept;
};

}  // namespace mcts

}  // namespace search

#endif
