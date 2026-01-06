#pragma once
#include <string>
#include "position.hpp"
#include "move.hpp"

namespace chess {

class Rules {
public:
    static bool is_pseudo_legal(const Position& pos, const Move& m, std::string& err);

    static bool is_square_attacked(const Position& pos, int sq, Color by);
    static bool in_check(const Position& pos, Color who);

    static bool is_castle_move(const Position& pos, const Move& m);
    static bool castle_path_safe(const Position& pos, const Move& m, std::string& err);

private:
    static bool pseudo_pawn(const Position& pos, Piece p, const Move& m, std::string& err);
    static bool pseudo_knight(const Position& pos, Piece p, int from, int to, std::string& err);
    static bool pseudo_bishop(const Position& pos, Piece p, int from, int to, std::string& err);
    static bool pseudo_rook  (const Position& pos, Piece p, int from, int to, std::string& err);
    static bool pseudo_queen (const Position& pos, Piece p, int from, int to, std::string& err);
    static bool pseudo_king  (const Position& pos, Piece p, int from, int to, std::string& err);

    static bool path_clear(const Position& pos, int from, int to);

    static bool is_opponent(Piece moving, Piece target);
    static bool is_friend(Piece moving, Piece target);

    static int  find_king(const Position& pos, Color who);
    static bool has_right(uint8_t rights, uint8_t flag) { return (rights & flag) != 0; }
};

} // namespace chess
