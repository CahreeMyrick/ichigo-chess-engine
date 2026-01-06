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
    // 1) White can castle king-side in a minimal empty position
    {
        auto pos = PosBuilder()
            .stm(Color::White)
            .piece("e1", Piece::WK)
            .piece("h1", Piece::WR)
            .piece("e8", Piece::BK) // keep a black king somewhere legal
            .build();

        pos.set_castling_rights(CR_WK);

        MoveList legal;
        MoveGen::generate_legal(pos, legal);
        assert(has(legal, "e1", "g1"));
    }

    // 2) Castling is illegal if king is in check (rook on e-file)
    {
        auto pos = PosBuilder()
            .stm(Color::White)
            .piece("e1", Piece::WK)
            .piece("h1", Piece::WR)
            .piece("e8", Piece::BR)
            .piece("a8", Piece::BK)
            .build();
        pos.set_castling_rights(CR_WK);

        MoveList legal;
        MoveGen::generate_legal(pos, legal);
        assert(!has(legal, "e1", "g1"));
    }

    // 3) Castling is illegal if passing square is attacked (attack f1)
    // Put a black rook on f8 with an open file to f1.
    {
        auto pos = PosBuilder()
            .stm(Color::White)
            .piece("e1", Piece::WK)
            .piece("h1", Piece::WR)
            .piece("f8", Piece::BR)
            .piece("a8", Piece::BK)
            .build();
        pos.set_castling_rights(CR_WK);

        // Ensure f-file squares are empty: f7..f2 empty by default (we didn't place anything)
        MoveList legal;
        MoveGen::generate_legal(pos, legal);
        assert(!has(legal, "e1", "g1"));
    }

    // 4) Applying castle actually moves rook to f1
    {
        auto pos = PosBuilder()
            .stm(Color::White)
            .piece("e1", Piece::WK)
            .piece("h1", Piece::WR)
            .piece("a8", Piece::BK)
            .build();
        pos.set_castling_rights(CR_WK);

        std::string err;
        bool ok = pos.make_move(MV("e1","g1"), err);
        if (!ok) { std::cerr << err << "\n"; assert(false); }

        assert(pos.at(SQ("g1")) == Piece::WK);
        assert(pos.at(SQ("f1")) == Piece::WR);
        assert(pos.at(SQ("h1")) == Piece::Empty);
    }

    std::cout << "test_castling: OK\n";
    return 0;
}

