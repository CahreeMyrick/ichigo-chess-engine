#include "support/testutil.hpp"

using namespace chess;
using namespace test;

int main() {
    // -------------------------
    // Basic L moves
    // -------------------------
    {
        auto pos = PosBuilder().piece("g1", Piece::WN).build();
        EXPECT_PSEUDO_OK(pos, "g1", "f3");
        EXPECT_PSEUDO_OK(pos, "g1", "h3");
        EXPECT_PSEUDO_OK(pos, "g1", "e2");
    }
    {
        auto pos = PosBuilder().piece("b1", Piece::WN).build();
        EXPECT_PSEUDO_OK(pos, "b1", "a3");
        EXPECT_PSEUDO_OK(pos, "b1", "c3");
        EXPECT_PSEUDO_OK(pos, "b1", "d2");
    }

    // -------------------------
    // Illegal shapes
    // -------------------------
    {
        auto pos = PosBuilder().piece("g1", Piece::WN).build();
        EXPECT_PSEUDO_BAD(pos, "g1", "g3"); // straight
        EXPECT_PSEUDO_BAD(pos, "g1", "f2"); // 1+1
        EXPECT_PSEUDO_BAD(pos, "g1", "h2"); // 1+1
    }

    // -------------------------
    // Captures
    // -------------------------
    {
        auto pos = PosBuilder()
            .piece("g1", Piece::WN)
            .piece("f3", Piece::BP)
            .build();
        EXPECT_PSEUDO_OK(pos, "g1", "f3"); // capture opponent
    }
    {
        auto pos = PosBuilder()
            .piece("g1", Piece::WN)
            .piece("f3", Piece::WP)
            .build();
        EXPECT_PSEUDO_BAD(pos, "g1", "f3"); // can't capture own
    }

    // -------------------------
    // Knights jump over pieces
    // (blockers should not matter)
    // -------------------------
    {
        auto pos = PosBuilder()
            .piece("g1", Piece::WN)
            .piece("g2", Piece::WP) // "in the way"
            .piece("f2", Piece::WP) // "in the way"
            .build();

        EXPECT_PSEUDO_OK(pos, "g1", "f3");
    }

    // -------------------------
    // Black knight
    // -------------------------
#ifdef TESTUTIL_HAS_STM
    {
        auto pos = PosBuilder().stm(Color::Black).piece("g8", Piece::BN).build();
        EXPECT_PSEUDO_OK(pos, "g8", "f6");
        EXPECT_PSEUDO_OK(pos, "g8", "h6");
        EXPECT_PSEUDO_OK(pos, "g8", "e7");
    }
#endif

    std::cout << "test_knight: OK\n";
    return 0;
}

