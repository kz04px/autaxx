#ifndef SEARCH_PV_HPP
#define SEARCH_PV_HPP

#include <libataxx/move.hpp>
#include <vector>

using PV = std::vector<libataxx::Move>;

inline bool legal_pv(const libataxx::Position &pos, const PV &pv) {
    libataxx::Position npos = pos;
    for (const auto &move : pv) {
        if (!npos.legal_move(move)) {
            return false;
        }
        npos.makemove(move);
    }
    return true;
}

#endif
