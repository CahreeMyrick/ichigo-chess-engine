#include "perft.hpp"
#include "movegen.hpp"

namespace chess {

uint64_t Perft::nodes(Position& pos, int depth) {
    if (depth <= 0) return 1ULL;

    MoveList moves;
    MoveGen::generate_legal(pos, moves);

    if (depth == 1) return (uint64_t)moves.size;

    uint64_t total = 0;
    for (int i = 0; i < moves.size; ++i) {
        std::string err;
        Position child = pos;
        if (!child.make_move(moves.moves[i], err)) continue; // should never fail if movegen is correct
        total += nodes(child, depth - 1);
    }
    return total;
}

} // namespace chess
