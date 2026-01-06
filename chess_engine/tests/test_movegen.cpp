#include "support/testutil.hpp"
#include "movegen.hpp"

using namespace chess;
using namespace test;

static bool contains_move(const MoveList& ml, const std::string& from, const std::string& to) {
    int f = SQ(from);
    int t = SQ(to);
    for (int i = 0; i < ml.size; ++i) {
        if (ml.moves[i].from == f && ml.moves[i].to == t) return true;
    }
    return false;
}

int main() {
    // 1) Start position: white has 20 legal moves (16 pawn + 4 knight).
    // BUT: our engine currently allows pawn double moves and knight moves, plus sliders/king are blocked.
    // Actually in startpos, sliders/king have no moves, so legal count should be 20.
    {
        Position pos = Position::startpos();
        MoveList legal;
        MoveGen::generate_legal(pos, legal);

        if (legal.size != 20) {
            std::cerr << "Expected 20 legal moves from startpos, got " << legal.size << "\n";
            assert(false);
        }

        assert(contains_move(legal, "e2", "e4"));
        assert(contains_move(legal, "g1", "f3"));
    }

    // 2) Simple rook pin-ish legality: king on e1, rook on e2, enemy rook on e8.
    // Moving rook away from e-file would expose check -> should be filtered out.
    {
        Position pos;
        pos.set(SQ("e1"), Piece::WK);
        pos.set(SQ("e2"), Piece::WR);
        pos.set(SQ("e8"), Piece::BR);

        MoveList legal;
        MoveGen::generate_legal(pos, legal);

        // Rook moves like e2->e3/e4/... are OK if still blocking? Actually moving up the file still blocks until it leaves.
        // BUT moving off file like e2->d2 should be illegal (exposes check).
        // Our rook movegen will generate many rook moves.
        assert(!contains_move(legal, "e2", "d2"));
        assert(!contains_move(legal, "e2", "f2"));

        // Staying on e-file should be allowed (still blocks)
        assert(contains_move(legal, "e2", "e3"));
    }

    // 3) has_any_legal_move sanity: king boxed by checkmate-like shape (not full mate logic yet)
    {
        Position pos;
        pos.set(SQ("e1"), Piece::WK);
        pos.set(SQ("e8"), Piece::BR);

        // White king has some moves, but many might still be legal depending on attacks.
        bool any = MoveGen::has_any_legal_move(pos);
        assert(any);
    }

    std::cout << "test_movegen: OK\n";
    return 0;
}

