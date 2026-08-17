# C++ Schach

Ein Schachspiel für 2 Spieler mit vollständigen Regeln: Zugvalidierung für alle Figuren, Rochade, En Passant, Bauernumwandlung sowie Erkennung von Schach, Schachmatt und Patt.

**[Im Browser spielen](https://bigpython0.github.io/Chess/)**

Die komplette Spiellogik habe ich von Grund auf selbst in C++17 geschrieben. Ursprünglich lief das Spiel nur als natives SFML-Fenster. Die Trennung der Logik von der SFML-Oberfläche sowie den WebAssembly-Port für den Browser habe ich mit Unterstützung von Claude (Anthropic) umgesetzt.

## Struktur

- `src/Board.h`, `src/Piece.h` – Spiellogik, unabhängig von SFML
- `src/main.cpp` – Desktop-Version mit SFML
- `src/wasm_bindings.cpp` – bindet dieselbe Logik per Embind an JavaScript
- `web/` – Browser-Frontend (HTML/CSS/JS)

## Ausführen

Desktop (benötigt SFML 3):

```bash
cmake -B build && cmake --build build && ./build/chess_game
```

Web (benötigt das Emscripten SDK):

```bash
./build_wasm.sh
cd web && python3 -m http.server 8000
```
