#pragma once
#include <cassert>
#include <string>
#include <vector>
#include <iostream>

#include "position.hpp"
#include "rules.hpp"
#include "parse.hpp"
#include "move.hpp"
#include "types.hpp"

#define TESTUTIL_HAS_STM

namespace test {

inline int SQ(const std::string& s) {
    auto sq = chess::Parse::square(s);
    if (!sq) {
        std::cerr << "Bad square string: " << s << "\n";
        assert(false);
    }
    return *sq;
}

inline chess::Move MV(const std::string& from, const std::string& to) {
    chess::Move m;
    m.from = static_cast<uint8_t>(SQ(from));
    m.to   = static_cast<uint8_t>(SQ(to));
    return m;
}

inline chess::Move MV(const std::string& from, const std::string& to, char promo) {
    chess::Move m;
    m.from = static_cast<uint8_t>(SQ(from));
    m.to   = static_cast<uint8_t>(SQ(to));
    switch (promo) {
        case 'q': case 'Q': m.promo = chess::PROMO_Q; break;
        case 'r': case 'R': m.promo = chess::PROMO_R; break;
        case 'b': case 'B': m.promo = chess::PROMO_B; break;
        case 'n': case 'N': m.promo = chess::PROMO_N; break;
        default: m.promo = chess::PROMO_NONE; break;
    }
    return m;
}


inline void EXPECT_PSEUDO_OK(const chess::Position& pos, const std::string& from, const std::string& to) {
    std::string err;
    bool ok = chess::Rules::is_pseudo_legal(pos, MV(from,to), err);
    if (!ok) std::cerr << "Expected OK " << from << "->" << to << " but got: " << err << "\n";
    assert(ok);
}

inline void EXPECT_PSEUDO_BAD(const chess::Position& pos, const std::string& from, const std::string& to) {
    std::string err;
    bool ok = chess::Rules::is_pseudo_legal(pos, MV(from,to), err);
    if (ok) std::cerr << "Expected BAD " << from << "->" << to << " but was OK\n";
    assert(!ok);
}

struct PosBuilder {
    chess::Position p;

    PosBuilder& piece(const std::string& sq, chess::Piece pc) {
        p.set(SQ(sq), pc);
        return *this;
    }

    PosBuilder& stm(chess::Color c) {
    	p.set_side_to_move(c);
    	return *this;
    }

    // PosBuilder& stm(chess::Color c) { p.set_side_to_move(c); return *this; }

    chess::Position build() const { return p; }
};

} // namespace test

