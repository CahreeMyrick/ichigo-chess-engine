#include <cassert>
#include <iostream>
#include "support/testutil.hpp"
#include "movegen.hpp"

using namespace chess;
using namespace test;

static bool has(const MoveList& ml, const std::string& a, const std::string& b) {
    int from = SQ(a), to = SQ(b);
    for (int i = 0; i < ml.size; ++i) {
        if (ml.moves[i].from == from && ml.moves[i].to == to) return true;
    }
    return false;
}

int main() {
    // EP creation + capture
    {
        // Kings placed so position is valid-ish
        auto pos = PosBuilder()
            .stm(Color::Black)
            .piece("e1", Piece::WK)
            .piece("a8", Piece::BK)
            .piece("e5", Piece::WP)
            .piece("d7", Piece::BP)
            .build();

        pos.set_ep_square(-1);

        std::string err;

        // Black plays d7->d5 (double). EP square should become d6.
        assert(pos.make_move(MV("d7","d5"), err) && err.empty());
        assert(pos.ep_square() == SQ("d6"));

        // White should have EP move e5->d6 available
        MoveList legal;
        MoveGen::generate_legal(pos, legal);
        assert(has(legal, "e5", "d6"));

        // Apply EP capture
        assert(pos.make_move(MV("e5","d6"), err) && err.empty());
        assert(pos.at(SQ("d6")) == Piece::WP);
        assert(pos.at(SQ("d5")) == Piece::Empty); // captured pawn removed
    }

    // EP expires if not used immediately
    {
        auto pos = PosBuilder()
            .stm(Color::Black)
            .piece("e1", Piece::WK)
            .piece("a8", Piece::BK)
            .piece("e5", Piece::WP)
            .piece("d7", Piece::BP)
            .piece("h7", Piece::BP) // a different pawn to move instead
            .build();

        pos.set_ep_square(-1);

        std::string err;
        assert(pos.make_move(MV("d7","d5"), err) && err.empty());
        assert(pos.ep_square() == SQ("d6"));

        // White makes a different move (no EP). We'll do king move e1->e2 (legal here).
        assert(pos.make_move(MV("e1","e2"), err) && err.empty());

        // EP should be cleared now (because white did not double-push a pawn)
        assert(pos.ep_square() == -1);

        // Black to move: EP capture is no longer available anyway
        // (We mainly want to ensure it doesn't persist incorrectly.)
    }

    std::cout << "test_enpassant: OK\n";
    return 0;
}

