#!/bin/bash
set -e

# Kompiliert die entkoppelte Chess-Engine (Board.h/Piece.h) zu WebAssembly.
# Benötigt das Emscripten SDK (emcc): https://emscripten.org/docs/getting_started/downloads.html

em++ src/wasm_bindings.cpp \
    -std=c++17 \
    -O2 \
    --bind \
    -o web/chess.js

echo "Fertig: web/chess.js und web/chess.wasm"
