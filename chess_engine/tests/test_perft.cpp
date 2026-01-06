#include <cassert>
#include <iostream>
#include "perft.hpp"
#include "position.hpp"

using namespace chess;

int main() {
    Position pos = Position::startpos();

    uint64_t n1 = Perft::nodes(pos, 1);
    uint64_t n2 = Perft::nodes(pos, 2);
    uint64_t n3 = Perft::nodes(pos, 3);
    uint64_t n4 = Perft::nodes(pos, 4);

    std::cout << "perft(1)=" << n1 << "\n";
    std::cout << "perft(2)=" << n2 << "\n";
    std::cout << "perft(3)=" << n3 << "\n";
    std::cout << "perft(4)=" << n4 << "\n";

    assert(n1 == 20);
    assert(n2 == 400);
    assert(n3 == 8902);
    assert(n4 == 197281);

    std::cout << "test_perft: OK\n";
    return 0;
}
