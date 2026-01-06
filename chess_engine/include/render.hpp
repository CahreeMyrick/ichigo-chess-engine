#pragma once
#include <string>
#include "position.hpp"

namespace chess {

class Render {
public:
    // ASCII board with coordinates
    static std::string board_ascii(const Position& pos);

    // single character for piece (KQBNRP / kqbnrp / '.')
    static char piece_char(Piece p);
};

} // namespace chess

