#pragma once
#include <array>
#include "move.hpp"

namespace chess {

struct MoveList {
    std::array<Move, 256> moves{};
    int size = 0;

    void clear() { size = 0; }
    void push(const Move& m) { moves[size++] = m; }
};

} // namespace chess

