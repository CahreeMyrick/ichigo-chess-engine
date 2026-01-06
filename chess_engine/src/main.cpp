#include <iostream>
#include <string>
#include <optional>

#include "position.hpp"
#include "render.hpp"
#include "parse.hpp"
#include "move.hpp"

#include "movegen.hpp"
#include "search.hpp"   
#include "rules.hpp"    

static std::string sq_str(int sq) {
    char f = char('a' + chess::file_of(sq));
    char r = char('1' + chess::rank_of(sq));
    std::string s; s += f; s += r;
    return s;
}

static char promo_char(uint8_t promo) {
    using namespace chess;
    switch (promo) {
        case PROMO_Q: return 'q';
        case PROMO_R: return 'r';
        case PROMO_B: return 'b';
        case PROMO_N: return 'n';
        default: return '\0';
    }
}

static std::string move_str(const chess::Move& m) {
    std::string s = sq_str(m.from) + " " + sq_str(m.to);
    char pc = promo_char(m.promo);
    if (pc) { s += "="; s += pc; }
    return s;
}

static void print_status(const chess::Position& pos) {
    using namespace chess;

    MoveList legal;
    MoveGen::generate_legal(pos, legal);

    if (legal.size == 0) {
        if (Rules::in_check(pos, pos.side_to_move())) {
            std::cout << "CHECKMATE. Winner: " << (pos.side_to_move() == Color::White ? "Black" : "White") << "\n";
        } else {
            std::cout << "STALEMATE.\n";
        }
        return;
    }

    if (Rules::in_check(pos, pos.side_to_move())) {
        std::cout << "CHECK.\n";
    }
}

int main() {
    using namespace chess;

    Position pos = Position::startpos();

    std::cout << "Mode select:\n";
    std::cout << "  1) PvP (human vs human)\n";
    std::cout << "  2) PvE (human vs AI)\n";
    std::cout << "> ";

    int mode = 1;
    {
        std::string s;
        std::getline(std::cin, s);
        if (!s.empty()) mode = std::stoi(s);
        if (mode != 1 && mode != 2) mode = 1;
    }

    Color human_side = Color::White;
    int ai_depth = 4;

    if (mode == 2) {
        std::cout << "Play as (w/b)? [w] > ";
        std::string s;
        std::getline(std::cin, s);
        if (!s.empty() && (s[0] == 'b' || s[0] == 'B')) human_side = Color::Black;

        std::cout << "AI depth? [4] > ";
        std::getline(std::cin, s);
        if (!s.empty()) ai_depth = std::max(1, std::stoi(s));

        std::cout << "You are " << (human_side == Color::White ? "White" : "Black")
                  << ". AI depth=" << ai_depth << "\n";
    }

    auto print_help = []() {
        std::cout << "Enter moves like: e2 e4  or  e7 e8=q\n";
        std::cout << "Commands:\n";
        std::cout << "  q / quit / exit\n";
        std::cout << "  r / reset\n";
        std::cout << "  mode pvp | mode ai\n";
        std::cout << "  depth N          (ai mode)\n";
        std::cout << "  side w|b         (ai mode)\n";
        std::cout << "\n";
    };

    std::cout << Render::board_ascii(pos);
    print_help();
    print_status(pos);

    std::string line;
    while (true) {
        // If AI mode and it's AI's turn, move automatically
        if (mode == 2) {
            Color stm = pos.side_to_move();
            Color ai_side = other(human_side);
            if (stm == ai_side) {
                MoveList legal;
                MoveGen::generate_legal(pos, legal);

                if (legal.size == 0) {
                    // game over
                    print_status(pos);
                    break;
                }

                Position tmp = pos; // Search expects non-const reference
                SearchResult res = Search::minimax(tmp, ai_depth);

                std::string err;
                if (!pos.make_move(res.best, err)) {
                    std::cout << "AI produced illegal move?? " << err << "\n";
                    break;
                }

                std::cout << "AI plays: " << move_str(res.best) << " (score " << res.score << ")\n";
                std::cout << Render::board_ascii(pos);
                print_status(pos);
                continue;
            }
        }

        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        // global commands
        if (line == "q" || line == "quit" || line == "exit") break;
        if (line == "h" || line == "help") { print_help(); continue; }

        if (line == "r" || line == "reset") {
            pos = Position::startpos();
            std::cout << Render::board_ascii(pos);
            print_status(pos);
            continue;
        }

        // mode switching at runtime
        if (line == "mode pvp") {
            mode = 1;
            std::cout << "Mode: PvP\n";
            continue;
        }
        if (line == "mode ai") {
            mode = 2;
            std::cout << "Mode: PvE (AI)\n";
            continue;
        }

        // AI settings
        if (line.rfind("depth ", 0) == 0) {
            if (mode != 2) { std::cout << "depth only applies in AI mode.\n"; continue; }
            try {
                ai_depth = std::max(1, std::stoi(line.substr(6)));
                std::cout << "AI depth set to " << ai_depth << "\n";
            } catch (...) {
                std::cout << "Invalid depth.\n";
            }
            continue;
        }
        if (line.rfind("side ", 0) == 0) {
            if (mode != 2) { std::cout << "side only applies in AI mode.\n"; continue; }
            char c = line.size() >= 6 ? line[5] : 'w';
            if (c == 'w' || c == 'W') human_side = Color::White;
            else if (c == 'b' || c == 'B') human_side = Color::Black;
            else { std::cout << "Use side w or side b.\n"; continue; }
            std::cout << "Human is now " << (human_side == Color::White ? "White" : "Black") << "\n";
            continue;
        }

        // Parse move: support e2 e4 and promotions e7 e8=q / e7 e8 q / e7 e8 =q
        // We'll reuse Parse::two_squares for the squares, then parse promo char from the tail.
        auto parsed = Parse::two_squares(line);
        if (!parsed) {
            std::cout << "Invalid input. Use format: e2 e4  or  e7 e8=q\n";
            continue;
        }

        // promo parsing
        char promo = '\0';
        {
            // look for '='
            std::size_t eq = line.find('=');
            if (eq != std::string::npos && eq + 1 < line.size()) {
                char c = line[eq + 1];
                if (c=='q'||c=='Q'||c=='r'||c=='R'||c=='b'||c=='B'||c=='n'||c=='N') promo = c;
            } else {
                // else last non-space char
                std::size_t j = line.find_last_not_of(" \t\r\n");
                if (j != std::string::npos) {
                    char c = line[j];
                    if (c=='q'||c=='Q'||c=='r'||c=='R'||c=='b'||c=='B'||c=='n'||c=='N') promo = c;
                }
            }
        }

        Move m;
        m.from = static_cast<uint8_t>(parsed->first);
        m.to   = static_cast<uint8_t>(parsed->second);

        if (promo) {
            switch (promo) {
                case 'q': case 'Q': m.promo = PROMO_Q; break;
                case 'r': case 'R': m.promo = PROMO_R; break;
                case 'b': case 'B': m.promo = PROMO_B; break;
                case 'n': case 'N': m.promo = PROMO_N; break;
                default: m.promo = PROMO_NONE; break;
            }
        }

        // If AI mode, ensure it's human's turn
        if (mode == 2) {
            if (pos.side_to_move() != human_side) {
                std::cout << "It's not your turn (AI to move). Type 'help' for commands.\n";
                continue;
            }
        }

        std::string err;
        if (!pos.make_move(m, err)) {
            std::cout << "Illegal move: " << err << "\n";
            continue;
        }

        std::cout << Render::board_ascii(pos);
        print_status(pos);
    }

    return 0;
}
