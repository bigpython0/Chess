// Reihenfolge muss zum enum class Type in Piece.h passen
const PIECE_NAMES = ["pawn", "bishop", "knight", "rook", "queen", "king"];

const WHITE_SYMBOLS = { pawn: "♙", bishop: "♗", knight: "♘", rook: "♖", queen: "♕", king: "♔" };
const BLACK_SYMBOLS = { pawn: "♟", bishop: "♝", knight: "♞", rook: "♜", queen: "♛", king: "♚" };

const PROMOTION_TYPES = { queen: 4, rook: 3, bishop: 1, knight: 2 };

let game;
let squares = [];
let selected = null; // {x, y} oder null

function pieceSymbol(x, y) {
    const type = game.getPieceType(x, y);
    if (type === -1) return "";

    const name = PIECE_NAMES[type];
    const color = game.getPieceColor(x, y);
    return color === 0 ? WHITE_SYMBOLS[name] : BLACK_SYMBOLS[name];
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
    const statusText = document.getElementById("status");
    const currentPlayer = game.getCurrentTurn() === 0 ? "Weiß" : "Schwarz";

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

function render() {
    for (const square of squares) {
        const x = Number(square.dataset.x);
        const y = Number(square.dataset.y);

        square.textContent = pieceSymbol(x, y);
        square.classList.remove("selected", "possible-move", "white-piece", "black-piece");

        const color = game.getPieceColor(x, y);
        if (color === 0) square.classList.add("white-piece");
        else if (color === 1) square.classList.add("black-piece");
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

    updateStatusText();
}

function handlePromotionIfNeeded(x, y) {
    const movedType = game.getPieceType(x, y);
    const isPawn = movedType === 0;
    const onLastRank = y === 0 || y === 7;

    if (!isPawn || !onLastRank) return;

    let choice = prompt("Bauer umwandeln in: queen, rook, bishop oder knight", "queen");
    if (!(choice in PROMOTION_TYPES)) choice = "queen";

    game.promote(x, y, PROMOTION_TYPES[choice]);
}

function onSquareClick(event) {
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
            const moved = game.movePiece(selected.x, selected.y, x, y);
            selected = null;

            if (moved) {
                handlePromotionIfNeeded(x, y);
                game.switchTurn();
            }
        }
    }

    render();
}

Module.onRuntimeInitialized = () => {
    game = new Module.WebGame();
    buildBoard();
    render();
};
