#include "position.hpp"
#include "rules.hpp"

namespace chess {

Position::Position() = default;

static Piece promoted_piece(Piece pawn, uint8_t promo) {
    const bool white = (pawn == Piece::WP);
    uint8_t p = promo;
    if (p == PROMO_NONE) p = PROMO_Q;

    switch (p) {
        case PROMO_Q: return white ? Piece::WQ : Piece::BQ;
        case PROMO_R: return white ? Piece::WR : Piece::BR;
        case PROMO_B: return white ? Piece::WB : Piece::BB;
        case PROMO_N: return white ? Piece::WN : Piece::BN;
        default:      return white ? Piece::WQ : Piece::BQ;
    }
}

bool Position::make_move(const Move& m, std::string& err) {
    err.clear();

    if (!Rules::is_pseudo_legal(*this, m, err)) return false;

    Piece moving   = at(m.from);
    Piece captured = at(m.to);

    // ---- helpers for special moves ----
    bool castle = Rules::is_castle_move(*this, m);

    bool is_pawn = (moving == Piece::WP || moving == Piece::BP);

    // Promotion?
    bool is_promotion = false;
    if (is_pawn) {
        int last_rank = (moving == Piece::WP) ? 7 : 0;
        is_promotion = (rank_of(m.to) == last_rank);
    }

    auto promoted_piece = [&](Piece pawn, uint8_t promo) -> Piece {
        const bool white = (pawn == Piece::WP);
        uint8_t p = promo;
        if (p == PROMO_NONE) p = PROMO_Q; // default

        switch (p) {
            case PROMO_Q: return white ? Piece::WQ : Piece::BQ;
            case PROMO_R: return white ? Piece::WR : Piece::BR;
            case PROMO_B: return white ? Piece::WB : Piece::BB;
            case PROMO_N: return white ? Piece::WN : Piece::BN;
            default:      return white ? Piece::WQ : Piece::BQ;
        }
    };

    // En passant capture?
    bool is_ep_capture = false;
    int  ep_captured_sq = -1;

    if (is_pawn && captured == Piece::Empty) {
        int df = file_of(m.to) - file_of(m.from);
        int dr = rank_of(m.to) - rank_of(m.from);
        int dir = (moving == Piece::WP) ? +1 : -1;

        if (std::abs(df) == 1 && dr == dir && ep_ == m.to) {
            is_ep_capture = true;
            ep_captured_sq = make_sq(file_of(m.to), rank_of(m.to) - dir);
        }
    }

    // Extra castling legality (through-check) safety
    if (castle) {
        if (!Rules::castle_path_safe(*this, m, err)) return false;
    }

    //  --- Try on a copy and reject if it leaves own king in check ---
    Position tmp = *this;

    // apply on tmp
    tmp.set(m.to, moving);
    tmp.set(m.from, Piece::Empty);

    if (is_ep_capture) {
        tmp.set(ep_captured_sq, Piece::Empty);
    }

    if (castle) {
        int r = (moving == Piece::WK) ? 0 : 7;
        bool king_side = (file_of(m.to) == 6);
        if (king_side) {
            // rook h -> f
            tmp.set(make_sq(5, r), tmp.at(make_sq(7, r)));
            tmp.set(make_sq(7, r), Piece::Empty);
        } else {
            // rook a -> d
            tmp.set(make_sq(3, r), tmp.at(make_sq(0, r)));
            tmp.set(make_sq(0, r), Piece::Empty);
        }
    }

    if (is_promotion) {
        tmp.set(m.to, promoted_piece(moving, m.promo));
    }

    Color mover = side_to_move();
    if (Rules::in_check(tmp, mover)) {
        err = "Move is illegal: it leaves your king in check.";
        return false;
    }

    // ---- Commit on real position ----
    set(m.to, moving);
    set(m.from, Piece::Empty);

    if (is_ep_capture) {
        set(ep_captured_sq, Piece::Empty);
    }

    if (castle) {
        int r = (moving == Piece::WK) ? 0 : 7;
        bool king_side = (file_of(m.to) == 6);
        if (king_side) {
            set(make_sq(5, r), at(make_sq(7, r)));
            set(make_sq(7, r), Piece::Empty);
        } else {
            set(make_sq(3, r), at(make_sq(0, r)));
            set(make_sq(0, r), Piece::Empty);
        }
    }

    if (is_promotion) {
        set(m.to, promoted_piece(moving, m.promo));
    }

    //  --- Update castling rights ---
    {
        auto clear = [&](uint8_t flags) { cr_ = (uint8_t)(cr_ & ~flags); };

        // King moved (including castling)
        if (moving == Piece::WK) clear(CR_WK | CR_WQ);
        if (moving == Piece::BK) clear(CR_BK | CR_BQ);

        // Rook moved from its original square
        if (moving == Piece::WR) {
            if (m.from == make_sq(0,0)) clear(CR_WQ);
            if (m.from == make_sq(7,0)) clear(CR_WK);
        }
        if (moving == Piece::BR) {
            if (m.from == make_sq(0,7)) clear(CR_BQ);
            if (m.from == make_sq(7,7)) clear(CR_BK);
        }

        // Rook captured on its original square
        if (captured == Piece::WR) {
            if (m.to == make_sq(0,0)) clear(CR_WQ);
            if (m.to == make_sq(7,0)) clear(CR_WK);
        }
        if (captured == Piece::BR) {
            if (m.to == make_sq(0,7)) clear(CR_BQ);
            if (m.to == make_sq(7,7)) clear(CR_BK);
        }
    }

    // ---- Update en passant target square ----
    // Set only if a pawn moved two squares; otherwise clear.
    ep_ = -1;
    if (moving == Piece::WP || moving == Piece::BP) {
        int dr = rank_of(m.to) - rank_of(m.from);
        int dir = (moving == Piece::WP) ? +1 : -1;
        if (dr == 2 * dir) {
            ep_ = make_sq(file_of(m.from), rank_of(m.from) + dir);
        }
    }

    stm_ = other(stm_);
    return true;
}

Position Position::startpos() {
    Position p;

    // White pieces
    p.set(make_sq(0,0), Piece::WR);
    p.set(make_sq(1,0), Piece::WN);
    p.set(make_sq(2,0), Piece::WB);
    p.set(make_sq(3,0), Piece::WQ);
    p.set(make_sq(4,0), Piece::WK);
    p.set(make_sq(5,0), Piece::WB);
    p.set(make_sq(6,0), Piece::WN);
    p.set(make_sq(7,0), Piece::WR);
    for (int f = 0; f < 8; ++f) p.set(make_sq(f,1), Piece::WP);

    // Black pieces
    for (int f = 0; f < 8; ++f) p.set(make_sq(f,6), Piece::BP);
    p.set(make_sq(0,7), Piece::BR);
    p.set(make_sq(1,7), Piece::BN);
    p.set(make_sq(2,7), Piece::BB);
    p.set(make_sq(3,7), Piece::BQ);
    p.set(make_sq(4,7), Piece::BK);
    p.set(make_sq(5,7), Piece::BB);
    p.set(make_sq(6,7), Piece::BN);
    p.set(make_sq(7,7), Piece::BR);

    p.set_ep_square(-1);

    p.set_castling_rights(CR_WK | CR_WQ | CR_BK | CR_BQ);

    return p;
}

} // namespace chess

