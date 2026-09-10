# C++ Schach

schachspiel für 2 spieler, alle regeln drin: zugvalidierung für jede figur, rochade, en passant, bauernumwandlung, schach/schachmatt/patt-erkennung.

**[im browser spielen](https://bigpython0.github.io/Chess/)**

die spiellogik hab ich komplett selbst in c++17 geschrieben, lief anfangs nur als natives SFML-fenster. die trennung von logik und SFML-oberfläche + den webassembly-port für den browser hab ich mit claude umgesetzt.

## struktur

- `src/Board.h`, `src/Piece.h` – spiellogik, unabhängig von SFML
- `src/main.cpp` – desktop version mit SFML
- `src/wasm_bindings.cpp` – bindet dieselbe logik per embind an javascript
- `web/` – browser frontend (html/css/js)

## ausführen

desktop (braucht SFML 3):

```bash
cmake -B build && cmake --build build && ./build/chess_game
```

web (braucht das emscripten sdk):

```bash
./build_wasm.sh
cd web && python3 -m http.server 8000
```
