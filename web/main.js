// Reihenfolge muss zum enum class Type in Piece.h passen
const PIECE_NAMES = ["pawn", "bishop", "knight", "rook", "queen", "king"];

const WHITE_SYMBOLS = { pawn: "♙", bishop: "♗", knight: "♘", rook: "♖", queen: "♕", king: "♔" };
const BLACK_SYMBOLS = { pawn: "♟", bishop: "♝", knight: "♞", rook: "♜", queen: "♛", king: "♚" };

let game;
let squares = [];
let selected = null; // {x, y} oder null
let lastMove = null; // {from: {x, y}, to: {x, y}} oder null
let pendingPromotion = null; // {x, y} oder null, waehrend das Auswahl-Panel offen ist
let gameOver = false;
let capturedWhite = []; // Piece-Types der geschlagenen weissen Figuren
let capturedBlack = []; // Piece-Types der geschlagenen schwarzen Figuren

function pieceSymbol(x, y) {
    const type = game.getPieceType(x, y);
    if (type === -1) return "";

    const name = PIECE_NAMES[type];
    const color = game.getPieceColor(x, y);
    return color === 0 ? WHITE_SYMBOLS[name] : BLACK_SYMBOLS[name];
}

// liest den kompletten Brettzustand aus, um Zuege vorher/nachher zu vergleichen (fuer die Schlagfiguren-Erkennung)
function snapshotBoard() {
    const snapshot = [];
    for (let y = 0; y < 8; y++) {
        const row = [];
        for (let x = 0; x < 8; x++) {
            const type = game.getPieceType(x, y);
            row.push(type === -1 ? null : { type: type, color: game.getPieceColor(x, y) });
        }
        snapshot.push(row);
    }
    return snapshot;
}

// findet die durch einen Zug geschlagene Figur, falls es eine gibt (auch bei En Passant)
function findCapturedPiece(before, after, from, to) {
    if (before[to.y][to.x] !== null) {
        return before[to.y][to.x]; // normaler Schlagzug
    }

    const movingType = before[from.y][from.x].type;
    if (movingType !== 0) return null; // nur Bauern koennen abseits des Zielfelds schlagen (En Passant)

    for (let y = 0; y < 8; y++) {
        for (let x = 0; x < 8; x++) {
            if (x === from.x && y === from.y) continue;
            if (x === to.x && y === to.y) continue;
            if (before[y][x] !== null && after[y][x] === null) {
                return before[y][x];
            }
        }
    }
    return null;
}

function buildBoard() {
    const boardDiv = document.getElementById("board");

    for (let row = 7; row >= 0; row--) {
        for (let col = 0; col < 8; col++) {
            const square = document.createElement("div");
            square.className = "square " + ((row + col) % 2 === 0 ? "light" : "dark");
            square.dataset.x = col;
            square.dataset.y = row;
            square.dataset.rank = row + 1;
            square.dataset.file = String.fromCharCode(97 + col); // 97 = 'a'
            square.addEventListener("click", onSquareClick);

            boardDiv.appendChild(square);
            squares.push(square);
        }
    }
}

function squareAt(x, y) {
    return squares.find(square => Number(square.dataset.x) === x && Number(square.dataset.y) === y);
}

function updateStatusText() {
    const statusText = document.getElementById("status-text");
    const turnDot = document.getElementById("turn-dot");
    const currentPlayer = game.getCurrentTurn() === 0 ? "Weiß" : "Schwarz";

    turnDot.classList.remove("dot-white", "dot-black");
    turnDot.classList.add(game.getCurrentTurn() === 0 ? "dot-white" : "dot-black");

    if (game.isCheckmate()) {
        const winner = currentPlayer === "Weiß" ? "Schwarz" : "Weiß";
        statusText.textContent = "Schachmatt! " + winner + " gewinnt.";
    } else if (game.isStalemate()) {
        statusText.textContent = "Patt!";
    } else if (game.isKingChecked()) {
        statusText.textContent = currentPlayer + " ist am Zug (Schach!)";
    } else {
        statusText.textContent = currentPlayer + " ist am Zug";
    }
}

function updateGameOverOverlay() {
    const overlay = document.getElementById("game-over-overlay");
    const text = document.getElementById("game-over-text");

    if (game.isCheckmate()) {
        const winner = game.getCurrentTurn() === 0 ? "Schwarz" : "Weiß";
        text.textContent = "Schachmatt! " + winner + " gewinnt.";
        overlay.classList.remove("hidden");
        gameOver = true;
    } else if (game.isStalemate()) {
        text.textContent = "Patt!";
        overlay.classList.remove("hidden");
        gameOver = true;
    } else {
        overlay.classList.add("hidden");
        gameOver = false;
    }
}

function renderCapturedPieces() {
    const blackTray = document.getElementById("captured-black");
    const whiteTray = document.getElementById("captured-white");

    blackTray.textContent = "";
    capturedBlack.forEach(type => {
        const glyph = document.createElement("span");
        glyph.className = "black-piece";
        glyph.textContent = BLACK_SYMBOLS[PIECE_NAMES[type]];
        blackTray.appendChild(glyph);
    });

    whiteTray.textContent = "";
    capturedWhite.forEach(type => {
        const glyph = document.createElement("span");
        glyph.className = "white-piece";
        glyph.textContent = WHITE_SYMBOLS[PIECE_NAMES[type]];
        whiteTray.appendChild(glyph);
    });
}

function render() {
    for (const square of squares) {
        const x = Number(square.dataset.x);
        const y = Number(square.dataset.y);

        square.textContent = pieceSymbol(x, y);
        square.classList.remove("selected", "possible-move", "white-piece", "black-piece", "last-move");

        const color = game.getPieceColor(x, y);
        if (color === 0) square.classList.add("white-piece");
        else if (color === 1) square.classList.add("black-piece");

        if (lastMove && ((x === lastMove.from.x && y === lastMove.from.y) || (x === lastMove.to.x && y === lastMove.to.y))) {
            square.classList.add("last-move");
        }
    }

    if (selected !== null) {
        squareAt(selected.x, selected.y).classList.add("selected");

        for (let y = 0; y < 8; y++) {
            for (let x = 0; x < 8; x++) {
                if (game.isValidMove(selected.x, selected.y, x, y)) {
                    squareAt(x, y).classList.add("possible-move");
                }
            }
        }
    }

    renderCapturedPieces();
    updateStatusText();
    updateGameOverOverlay();
}

function finishTurn() {
    game.switchTurn();
    render();
}

function showPromotionOverlay(x, y) {
    pendingPromotion = { x: x, y: y };

    const color = game.getCurrentTurn(); // switchTurn() ist noch nicht aufgerufen -> Farbe des ziehenden Spielers
    const symbols = color === 0 ? WHITE_SYMBOLS : BLACK_SYMBOLS;

    document.querySelectorAll(".promotion-choice").forEach(button => {
        const type = Number(button.dataset.type);
        button.textContent = symbols[PIECE_NAMES[type]];
        button.classList.remove("white-piece", "black-piece");
        button.classList.add(color === 0 ? "white-piece" : "black-piece");
    });

    document.getElementById("promotion-overlay").classList.remove("hidden");
}

function hidePromotionOverlay() {
    pendingPromotion = null;
    document.getElementById("promotion-overlay").classList.add("hidden");
}

function choosePromotion(pieceType) {
    game.promote(pendingPromotion.x, pendingPromotion.y, pieceType);
    hidePromotionOverlay();
    finishTurn();
}

function onSquareClick(event) {
    if (gameOver || pendingPromotion !== null) return;

    const x = Number(event.currentTarget.dataset.x);
    const y = Number(event.currentTarget.dataset.y);

    if (selected === null) {
        const type = game.getPieceType(x, y);
        const color = game.getPieceColor(x, y);

        if (type !== -1 && color === game.getCurrentTurn()) {
            selected = { x: x, y: y };
        }
    } else {
        const targetColor = game.getPieceColor(x, y);

        if (targetColor === game.getCurrentTurn()) {
            // anderes eigenes Piece anklicken -> Auswahl wechseln
            selected = { x: x, y: y };
        } else {
            const fromPos = selected;
            const before = snapshotBoard();
            const moved = game.movePiece(fromPos.x, fromPos.y, x, y);
            selected = null;

            if (moved) {
                const after = snapshotBoard();
                const captured = findCapturedPiece(before, after, fromPos, { x: x, y: y });
                if (captured !== null) {
                    if (captured.color === 0) capturedWhite.push(captured.type);
                    else capturedBlack.push(captured.type);
                }

                lastMove = { from: fromPos, to: { x: x, y: y } };

                const isPromotion = game.getPieceType(x, y) === 0 && (y === 0 || y === 7);
                if (isPromotion) {
                    render();
                    showPromotionOverlay(x, y);
                    return;
                }

                finishTurn();
                return;
            }
        }
    }

    render();
}

Module.onRuntimeInitialized = () => {
    game = new Module.WebGame();
    buildBoard();

    document.querySelectorAll(".promotion-choice").forEach(button => {
        button.addEventListener("click", () => choosePromotion(Number(button.dataset.type)));
    });

    render();
};
