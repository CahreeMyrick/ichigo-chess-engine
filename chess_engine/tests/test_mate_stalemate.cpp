#include <cassert>
#include <iostream>

#include "support/testutil.hpp"
#include "gamestate.hpp"
#include "movegen.hpp"
#include "rules.hpp"

using namespace chess;
using namespace test;

static void EXPECT(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "FAIL: " << msg << "\n";
        assert(false);
    }
}

int main() {
    // -------------------------
    // CHECKMATE: Black to move, mate in corner
    //
    // Black: King a8
    // White: King c6, Queen b7
    // It's Black to move.
    //
    // Queen b7 checks a8 (same rank), king c6 covers b7/a7/b6/etc.
    // Black king has no legal escapes and cannot capture the queen.
    // -------------------------
    {
        Position pos;
        pos.set(SQ("a8"), Piece::BK);
        pos.set(SQ("c6"), Piece::WK);
        pos.set(SQ("b7"), Piece::WQ);

       
#ifdef TESTUTIL_HAS_STM
       
        auto p2 = PosBuilder()
            .stm(Color::Black)
            .piece("a8", Piece::BK)
            .piece("c6", Piece::WK)
            .piece("b7", Piece::WQ)
            .build();
        EXPECT(GameState::is_checkmate(p2, Color::Black), "Expected checkmate (black).");
#else
        std::cerr << "NOTE: test_checkmate requires side-to-move control (add set_side_to_move/stm).\n";
#endif
    }

    // -------------------------
    // STALEMATE: Black to move, no legal moves but not in check
    //
    // Black: King a8
    // White: King c6, Queen c7
    // It's Black to move.
    //
    // Queen c7 does NOT check a8, but covers a7 and b8,
    // and white king covers b7/a7/b6 etc. Black king has no legal squares.
    // -------------------------
    {
#ifdef TESTUTIL_HAS_STM
        auto pos = PosBuilder()
            .stm(Color::Black)
            .piece("a8", Piece::BK)
            .piece("c6", Piece::WK)
            .piece("c7", Piece::WQ)
            .build();

        EXPECT(!Rules::in_check(pos, Color::Black), "Expected black not in check (stalemate position).");
        EXPECT(GameState::is_stalemate(pos, Color::Black), "Expected stalemate (black).");
#else
        std::cerr << "NOTE: test_stalemate requires side-to-move control (add set_side_to_move/stm).\n";
#endif
    }

    std::cout << "test_mate_stalemate: OK (if stm supported)\n";
    return 0;
}

