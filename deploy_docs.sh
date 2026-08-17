#!/bin/bash
set -e

# Baut die Web-Version neu und kopiert sie nach docs/, von wo GitHub Pages sie ausliefert.
./build_wasm.sh
cp web/index.html web/style.css web/main.js web/chess.js web/chess.wasm docs/

echo "docs/ aktualisiert - git add/commit/push nicht vergessen"
