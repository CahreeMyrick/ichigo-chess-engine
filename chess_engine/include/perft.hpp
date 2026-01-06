#pragma once
#include <cstdint>
#include "position.hpp"

namespace chess {
struct Perft {
    static uint64_t nodes(Position& pos, int depth);
};
} // namespace chess
