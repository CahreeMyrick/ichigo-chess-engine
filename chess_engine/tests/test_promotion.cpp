#include <cassert>
#include <iostream>
#include "support/testutil.hpp"
#include "movegen.hpp"

using namespace chess;
using namespace test;

static bool has_promo(const MoveList& ml, const std::string& a, const std::string& b, uint8_t promo) {
    int from = SQ(a), to = SQ(b);
    for (int i = 0; i < ml.size; ++i) {
        const Move& m = ml.moves[i];
        if (m.from == from && m.to == to && m.promo == promo) return true;
    }
    return false;
}

int main() {
    // 1) Movegen: white pawn on e7 should generate 4 promotions to e8
    {
        auto pos = PosBuilder()
            .stm(Color::White)
            .piece("e1", Piece::WK)
            .piece("a8", Piece::BK)
            .piece("e7", Piece::WP)
            .build();

        MoveList legal;
        MoveGen::generate_legal(pos, legal);

        assert(has_promo(legal, "e7", "e8", PROMO_Q));
        assert(has_promo(legal, "e7", "e8", PROMO_R));
        assert(has_promo(legal, "e7", "e8", PROMO_B));
        assert(has_promo(legal, "e7", "e8", PROMO_N));
    }

    // 2) Applying a chosen promotion works (promote to knight)
    {
        auto pos = PosBuilder()
            .stm(Color::White)
            .piece("e1", Piece::WK)
            .piece("a8", Piece::BK)
            .piece("e7", Piece::WP)
            .build();

        std::string err;
        bool ok = pos.make_move(MV("e7","e8",'n'), err);
        if (!ok) { std::cerr << err << "\n"; assert(false); }

        assert(pos.at(SQ("e8")) == Piece::WN);
    }

    // 3) Capture-promotion generates 4 moves (d7 takes e8 = {q,r,b,n})
    {
        auto pos = PosBuilder()
            .stm(Color::White)
            .piece("e1", Piece::WK)
            .piece("a8", Piece::BK)
            .piece("d7", Piece::WP)
            .piece("e8", Piece::BR)
            .build();

        MoveList legal;
        MoveGen::generate_legal(pos, legal);

        assert(has_promo(legal, "d7", "e8", PROMO_Q));
        assert(has_promo(legal, "d7", "e8", PROMO_R));
        assert(has_promo(legal, "d7", "e8", PROMO_B));
        assert(has_promo(legal, "d7", "e8", PROMO_N));
    }

    std::cout << "test_promotion: OK\n";
    return 0;
}
