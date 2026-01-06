#include "support/testutil.hpp"

using namespace chess;
using namespace test;

static void EXPECT_IN_CHECK(const Position& pos, Color who) {
    if (!Rules::in_check(pos, who)) {
        std::cerr << "Expected in_check(" << (who==Color::White?"White":"Black") << ") but got false\n";
        assert(false);
    }
}

static void EXPECT_NOT_IN_CHECK(const Position& pos, Color who) {
    if (Rules::in_check(pos, who)) {
        std::cerr << "Expected NOT in_check(" << (who==Color::White?"White":"Black") << ") but got true\n";
        assert(false);
    }
}

int main() {
    // Simple rook check: White king on e1, Black rook on e8, empty file.
    {
        auto pos = PosBuilder()
            .piece("e1", Piece::WK)
            .piece("e8", Piece::BR)
            .build();

        EXPECT_IN_CHECK(pos, Color::White);
    }

    // Blocked rook check: add a blocker on e4
    {
        auto pos = PosBuilder()
            .piece("e1", Piece::WK)
            .piece("e8", Piece::BR)
            .piece("e4", Piece::WP)
            .build();

        EXPECT_NOT_IN_CHECK(pos, Color::White);
    }

    // Knight check: Black knight attacks e4 from f6 (f6->e4 is L)
    {
        auto pos = PosBuilder()
            .piece("e4", Piece::WK)
            .piece("f6", Piece::BN)
            .build();

        EXPECT_IN_CHECK(pos, Color::White);
    }

    // Pawn check: black pawn attacks e4 from d5 or f5 (black pawn attacks down)
    {
        auto pos = PosBuilder()
            .piece("e4", Piece::WK)
            .piece("d5", Piece::BP)
            .build();

        EXPECT_IN_CHECK(pos, Color::White);
    }

    std::cout << "test_check: OK\n";
    return 0;
}

