# Chess

A 2-player chess game written in C++17 with SFML 3. Move pieces with the
mouse; the engine enforces legal moves, check, checkmate, stalemate,
castling, and pawn promotion.

## Features

- Full move validation per piece type (pawn double-step/en passant-less
  capture rules, knight, bishop, rook, queen, king)
- Check, checkmate, and stalemate detection
- Castling (kingside/queenside)
- Pawn promotion with an in-game selection UI
- Move animation and board/turn highlighting
- Right-click square highlighting for annotating the board

## Project structure

```
Chess/
├── src/
│   ├── main.cpp     # SFML rendering, input handling, game loop
│   ├── Board.h       # Board state and move/check/checkmate logic (SFML-free)
│   └── Piece.h        # Piece hierarchy and per-piece move rules (SFML-free)
├── assets/
│   ├── images/        # Piece sprites and board texture
│   └── fonts/          # UI font
├── CMakeLists.txt
└── LICENSE
```

`Board.h` and `Piece.h` contain the game logic decoupled from SFML (using a
plain `Position { int x, y; }` struct instead of `sf::Vector2i`). This split
exists so the engine can be reused outside the desktop app, e.g. compiled to
WebAssembly with a JS/canvas front end instead of SFML.

## Building

### Requirements

- CMake ≥ 3.16
- A C++17 compiler
- [SFML 3](https://www.sfml-dev.org/)

### macOS (Homebrew)

```bash
brew install sfml cmake
cmake -B build
cmake --build build
./build/chess_game
```

Run the binary from the repository root — asset paths (`assets/...`) are
resolved relative to the working directory.

### Linux / Windows

Install SFML 3 via your package manager or from source, then use the same
`cmake -B build && cmake --build build` flow. On Windows with vcpkg:

```powershell
cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

A VS Code build task (`.vscode/tasks.json`) is also included for a quick
`clang++` build on macOS.

## Roadmap

- [ ] WebAssembly build using the decoupled `Board`/`Piece` engine
- [ ] En passant
- [ ] Move history / undo

## License

MIT — see [LICENSE](LICENSE).
