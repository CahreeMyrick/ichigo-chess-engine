#include "support/testutil.hpp"

using namespace chess;
using namespace test;

int main() {
    // -------------------------
    // Bishop: diagonal movement
    // -------------------------
    {
        auto pos = PosBuilder().piece("c1", Piece::WB).build();
        EXPECT_PSEUDO_OK(pos, "c1", "h6");
        EXPECT_PSEUDO_OK(pos, "c1", "b2");
        EXPECT_PSEUDO_BAD(pos, "c1", "c2"); // not diagonal
    }
    {
        auto pos = PosBuilder()
            .piece("c1", Piece::WB)
            .piece("d2", Piece::WP) // block
            .build();
        EXPECT_PSEUDO_BAD(pos, "c1", "h6");
    }
    {
        auto pos = PosBuilder()
            .piece("c1", Piece::WB)
            .piece("h6", Piece::BP) // capture allowed if path clear
            .build();
        EXPECT_PSEUDO_OK(pos, "c1", "h6");
    }

    // -------------------------
    // Rook: straight movement
    // -------------------------
    {
        auto pos = PosBuilder().piece("a1", Piece::WR).build();
        EXPECT_PSEUDO_OK(pos, "a1", "a8");
        EXPECT_PSEUDO_OK(pos, "a1", "h1");
        EXPECT_PSEUDO_BAD(pos, "a1", "b2"); // not straight
    }
    {
        auto pos = PosBuilder()
            .piece("a1", Piece::WR)
            .piece("a3", Piece::WP) // block file
            .build();
        EXPECT_PSEUDO_BAD(pos, "a1", "a8");
    }

    // -------------------------
    // Queen: rook or bishop
    // -------------------------
    {
        auto pos = PosBuilder().piece("d1", Piece::WQ).build();
        EXPECT_PSEUDO_OK(pos, "d1", "d8"); // rook-like
        EXPECT_PSEUDO_OK(pos, "d1", "h5"); // bishop-like
        EXPECT_PSEUDO_BAD(pos, "d1", "e3"); // not straight/diag
    }
    {
        auto pos = PosBuilder()
            .piece("d1", Piece::WQ)
            .piece("d4", Piece::WP) // blocks file
            .build();
        EXPECT_PSEUDO_BAD(pos, "d1", "d8");
    }

    std::cout << "test_sliders: OK\n";
    return 0;
}

