# C++ Schachspiel

Ein Schachspiel für 2 Spieler, entwickelt mit C++17 und SFML 3.

Ich habe dieses Projekt umgesetzt, um **objektorientierte Programmierung (OOP)**, saubere Softwarearchitektur und die Implementierung komplexer Spiel-Logik von Grund auf zu vertiefen.

---

## Features

- **Vollständige Schachregeln:** Gültige Zug-Validierung für alle Figuren sowie Erkennung von Schach, Schachmatt und Patt.
- **Sonderzüge:** Rochade und Bauernumwandlung (mit eigener Auswahl-UI im Spiel).
- **Visuelles & Steuerung:** Sanfte Figuren-Animationen, Zug-Hervorhebung und Feld-Markierungen per Rechtsklick.

---

## Code-Struktur

Um den Code sauber zu strukturieren, ist die Spiel-Logik komplett von der SFML-Grafik getrennt:

- `src/Board.h` & `src/Piece.h` – Reine C++-Schachlogik (vollkommen unabhängig von SFML).
- `src/main.cpp` – Verwaltet das SFML-Fenster, das Rendering und die Benutzereingaben.
- `src/wasm_bindings.cpp` – Bindet dieselbe Logik per Embind an JavaScript.

---

## Web-Version (WebAssembly)

Weil die Spiel-Logik unabhängig von SFML ist, läuft sie auch im Browser: `web/`
enthält ein einfaches HTML/CSS/JS-Frontend, das die zu WebAssembly kompilierte
Engine steuert (Unicode-Symbole statt Sprites, kein SFML nötig).

Build mit dem [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html):

```bash
./build_wasm.sh
cd web && python3 -m http.server 8000
```

Danach `http://localhost:8000` im Browser öffnen (nicht per `file://` — WebAssembly braucht einen Server).
