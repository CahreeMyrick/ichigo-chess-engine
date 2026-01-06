#pragma once
#include <cstdint>

namespace chess {

enum class Color : uint8_t { White = 0, Black = 1 };

inline Color other(Color c) {
    return (c == Color::White) ? Color::Black : Color::White;
}

// Compact piece codes. Empty is 0 so default-initialized arrays are empty.
enum class Piece : uint8_t {
    Empty = 0,
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK
};

inline bool is_empty(Piece p) { return p == Piece::Empty; }
inline bool is_white(Piece p) { return p >= Piece::WP && p <= Piece::WK; }
inline bool is_black(Piece p) { return p >= Piece::BP && p <= Piece::BK; }

// Square indexing: 0 = a1, 7 = h1, 56 = a8, 63 = h8
inline int file_of(int sq) { return sq & 7; }        // 0..7
inline int rank_of(int sq) { return sq >> 3; }       // 0..7
inline int make_sq(int file, int rank) { return (rank << 3) | file; }

} // namespace chess

