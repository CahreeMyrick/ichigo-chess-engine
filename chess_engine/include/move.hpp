#pragma once
#include <cstdint>

namespace chess {

enum Promo : uint8_t {
    PROMO_NONE = 0,
    PROMO_Q = 1,
    PROMO_R = 2,
    PROMO_B = 3,
    PROMO_N = 4,
};

struct Move {
    uint8_t from = 0;
    uint8_t to   = 0;
    uint8_t promo = PROMO_NONE; // promotion choice
};

} // namespace chess

