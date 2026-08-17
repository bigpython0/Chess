# C++ Schachspiel

Ein Schachspiel für 2 Spieler, entwickelt mit C++17 und SFML 3.

Ich habe dieses Projekt umgesetzt, um **objektorientierte Programmierung (OOP)**, saubere Softwarearchitektur und die Implementierung komplexer Spiel-Logik von Grund auf zu vertiefen.

---

## 🎮 Features

- **Vollständige Schachregeln:** Gültige Zug-Validierung für alle Figuren sowie Erkennung von Schach, Schachmatt und Patt.
- **Sonderzüge:** Rochade und Bauernumwandlung (mit eigener Auswahl-UI im Spiel).
- **Visuelles & Steuerung:** Sanfte Figuren-Animationen, Zug-Hervorhebung und Feld-Markierungen per Rechtsklick.

---

## 🛠️ Code-Struktur

Um den Code sauber zu strukturieren, ist die Spiel-Logik komplett von der SFML-Grafik getrennt:

- `src/Board.h` & `src/Piece.h` – Reine C++-Schachlogik (vollkommen unabhängig von SFML).
- `src/main.cpp` – Verwaltet das SFML-Fenster, das Rendering und die Benutzereingaben.

> **Hinweis:** Durch die Entkopplung der Logik von SFML lässt sich die Engine später problemlos nach **WebAssembly (WASM)** kompilieren, um direkt im Browser zu laufen!

---

## 🚀 Kompilieren und Starten

### Voraussetzungen
- C++17-fähiger Compiler
- CMake (>= 3.16)
- SFML 3

### Build-Schritte (macOS / Linux)
```bash
cmake -B build
cmake --build build
./build/chess_game
