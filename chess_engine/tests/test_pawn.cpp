#include "support/testutil.hpp"

using namespace chess;
using namespace test;

int main() {
    // -------------------------
    // White pawn forward moves
    // -------------------------
    {
        auto pos = PosBuilder().piece("e2", Piece::WP).build();
        EXPECT_PSEUDO_OK(pos, "e2", "e3");
    }
    {
        auto pos = PosBuilder().piece("e2", Piece::WP).build();
        EXPECT_PSEUDO_OK(pos, "e2", "e4"); // double from start
    }
    {
        auto pos = PosBuilder().piece("e3", Piece::WP).build();
        EXPECT_PSEUDO_BAD(pos, "e3", "e5"); // not from start
    }
    {
        auto pos = PosBuilder().piece("e2", Piece::WP).piece("e3", Piece::BP).build();
        EXPECT_PSEUDO_BAD(pos, "e2", "e3"); // blocked
    }
    {
        auto pos = PosBuilder().piece("e2", Piece::WP).piece("e4", Piece::BP).build();
        EXPECT_PSEUDO_BAD(pos, "e2", "e4"); // landing square occupied
    }
    {
        auto pos = PosBuilder().piece("e2", Piece::WP).piece("e3", Piece::BP).build();
        EXPECT_PSEUDO_BAD(pos, "e2", "e4"); // intermediate blocked
    }

    // -------------------------
    // White pawn captures
    // -------------------------
    {
        auto pos = PosBuilder()
            .piece("e4", Piece::WP)
            .piece("d5", Piece::BP)
            .build();
        EXPECT_PSEUDO_OK(pos, "e4", "d5");
    }
    {
        auto pos = PosBuilder()
            .piece("e4", Piece::WP)
            .piece("f5", Piece::BP)
            .build();
        EXPECT_PSEUDO_OK(pos, "e4", "f5");
    }
    {
        auto pos = PosBuilder().piece("e4", Piece::WP).build();
        EXPECT_PSEUDO_BAD(pos, "e4", "d5"); // diagonal must capture (no EP yet)
    }
    {
        auto pos = PosBuilder()
            .piece("e4", Piece::WP)
            .piece("d5", Piece::WP)
            .build();
        EXPECT_PSEUDO_BAD(pos, "e4", "d5"); // can't capture own piece
    }

    // -------------------------
    // White pawn wrong direction
    // -------------------------
    {
        auto pos = PosBuilder().piece("e4", Piece::WP).build();
        EXPECT_PSEUDO_BAD(pos, "e4", "e3"); // backward
    }

    // -------------------------
    // Optional Black pawn tests
    // -------------------------
#ifdef TESTUTIL_HAS_STM
    {
        auto pos = PosBuilder().stm(Color::Black).piece("e7", Piece::BP).build();
        EXPECT_PSEUDO_OK(pos, "e7", "e6");
        EXPECT_PSEUDO_OK(pos, "e7", "e5");
    }
    {
        auto pos = PosBuilder().stm(Color::Black).piece("e6", Piece::BP).build();
        EXPECT_PSEUDO_BAD(pos, "e6", "e4"); // not from start
    }
    {
        auto pos = PosBuilder().stm(Color::Black).piece("e5", Piece::BP).piece("d4", Piece::WP).build();
        EXPECT_PSEUDO_OK(pos, "e5", "d4"); // capture down-left
    }
    {
        auto pos = PosBuilder().stm(Color::Black).piece("e5", Piece::BP).build();
        EXPECT_PSEUDO_BAD(pos, "e5", "d4"); // must capture
    }
#endif

    std::cout << "test_pawn: OK\n";
    return 0;
}

