#pragma once
#include "position.hpp"

namespace chess {

struct Eval {
    static int evaluate(const Position& pos);
};

}
