#ifndef PV_HPP
#define PV_HPP

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <vector>

typedef std::vector<libataxx::Move> PV;

// Check the legality of a PV based on a given board
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
