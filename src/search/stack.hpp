#ifndef SEARCH_STACK_HPP
#define SEARCH_STACK_HPP

#include "pv.hpp"

namespace search {

struct Stack {
    int ply = 0;
    PV pv;
};

}  // namespace search

#endif
