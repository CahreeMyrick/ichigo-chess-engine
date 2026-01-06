#pragma once
#include <array>
#include <string>
#include "types.hpp"
#include "move.hpp"

namespace chess {

enum CastlingRights : uint8_t {
    CR_NONE = 0,
    CR_WK = 1 << 0, // White king-side
    CR_WQ = 1 << 1, // White queen-side
    CR_BK = 1 << 2, // Black king-side
    CR_BQ = 1 << 3  // Black queen-side
};

class Position {
public:
    Position();
    static Position startpos();

    Piece at(int sq) const { return board_[sq]; }
    void set(int sq, Piece p) { board_[sq] = p; }

    Color side_to_move() const { return stm_; }
    void set_side_to_move(Color c) { stm_ = c; }

    bool make_move(const Move& m, std::string& err);

    uint8_t castling_rights() const { return cr_; }
    void set_castling_rights(uint8_t cr) { cr_ = cr; }
    
    int ep_square() const { return ep_; }          // -1 if none
    void set_ep_square(int sq) { ep_ = sq; }       // set to -1 to clear



private:
    std::array<Piece, 64> board_{};
    Color stm_ = Color::White;
    uint8_t cr_ = CR_NONE;
    int ep_ = -1;  // en passant target square (the square "passed over"), or -1
};

} // namespace chess

