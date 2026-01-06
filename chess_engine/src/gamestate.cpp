#include "gamestate.hpp"
#include "rules.hpp"
#include "movegen.hpp"

namespace chess {

bool GameState::is_checkmate(const Position& pos, Color who) {
    // Checkmate = in check + no legal moves
    return Rules::in_check(pos, who) && !MoveGen::has_any_legal_move(pos);
}

bool GameState::is_stalemate(const Position& pos, Color who) {
    // Stalemate = not in check + no legal moves
    return !Rules::in_check(pos, who) && !MoveGen::has_any_legal_move(pos);
}

} // namespace chess

