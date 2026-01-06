#pragma once
#include "position.hpp"
#include "move.hpp"

namespace chess {

struct SearchResult {
    Move best;
    int score;
};

struct Search {
    static SearchResult minimax(Position& pos, int depth);
};

}
