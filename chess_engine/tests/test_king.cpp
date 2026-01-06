#include "support/testutil.hpp"

using namespace chess;
using namespace test;

int main() {
    // -------------------------
    // King: basic adjacent moves
    // -------------------------
    {
        auto pos = PosBuilder().piece("e4", Piece::WK).build();
        EXPECT_PSEUDO_OK(pos, "e4", "e5");
        EXPECT_PSEUDO_OK(pos, "e4", "e3");
        EXPECT_PSEUDO_OK(pos, "e4", "f4");
        EXPECT_PSEUDO_OK(pos, "e4", "d4");
        EXPECT_PSEUDO_OK(pos, "e4", "f5");
        EXPECT_PSEUDO_OK(pos, "e4", "d5");
        EXPECT_PSEUDO_OK(pos, "e4", "f3");
        EXPECT_PSEUDO_OK(pos, "e4", "d3");
    }

    // -------------------------
    // King: illegal distances
    // -------------------------
    {
        auto pos = PosBuilder().piece("e4", Piece::WK).build();
        EXPECT_PSEUDO_BAD(pos, "e4", "e6"); // too far
        EXPECT_PSEUDO_BAD(pos, "e4", "g4"); // too far
        EXPECT_PSEUDO_BAD(pos, "e4", "c2"); // too far
    }

    // -------------------------
    // King: captures
    // -------------------------
    {
        auto pos = PosBuilder()
            .piece("e4", Piece::WK)
            .piece("f5", Piece::BP)
            .build();
        EXPECT_PSEUDO_OK(pos, "e4", "f5"); // capture opponent
    }
    {
        auto pos = PosBuilder()
            .piece("e4", Piece::WK)
            .piece("f5", Piece::WP)
            .build();
        EXPECT_PSEUDO_BAD(pos, "e4", "f5"); // cannot capture own
    }

    std::cout << "test_king: OK\n";
    return 0;
}

