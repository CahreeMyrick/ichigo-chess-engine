# Ichigo Chess Engine
A modern C++ chess engine that focuses correctness and clean design. The engine supports  **full legal chess rules**, **perft-verified move generation**,  and a **minimax AI with alpa-beta pruning**, playable via a simple CLI.

This project was built deliberately **without external chess libraries** to practice large software system design and developemnt with C++.

## Features

### Complete Chess Rules
- Legal move generation for **all pieces**
- **Check / checkmate / stalemate** detection
- **Castling** 
- **En passant**
- **Promotion** 

### Verified Correctness
- **Perft-tested** (https://www.chessprogramming.org/Perft) against known reference values  

### AI Engine
- **Minimax search with alpha–beta pruning**
- Configurable search depth
- Simple material-based evaluation

### CLI Interface
- **PvP** (human vs human)
- **PvE** (human vs AI)
- Runtime commands:
  - reset board
  - change AI depth
  - switch sides

### Build Instructions
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Run
```bash 
./ichigo_main
```

### Run Tests
```bash
./test_perft
./test_castling
./test_enpassant
./test_promotion
```

### Demo
https://github.com/user-attachments/assets/6587a5c0-5984-49c7-b240-9177d6db2449
