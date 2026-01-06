#pragma once
#include "position.hpp"

namespace chess {

class GameState {
public:
    static bool is_checkmate(const Position& pos, Color who);
    static bool is_stalemate(const Position& pos, Color who);
};

} // namespace chess

