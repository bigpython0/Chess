#include <emscripten/bind.h>
#include "Board.h"

using namespace emscripten;

class WebGame {
    private:
        Board board;

    public:
        bool movePiece(int fromX, int fromY, int toX, int toY) {
            return board.movePiece({fromX, fromY}, {toX, toY});
        }

        bool isValidMove(int fromX, int fromY, int toX, int toY) {
            return board.isMoveLegal({fromX, fromY}, {toX, toY});
        }

        void promote(int x, int y, int pieceType) {
            Piece::Color color = board.getCurrentTurn();
            Piece::Type type = static_cast<Piece::Type>(pieceType);

            if (type == Piece::Type::Queen) {
                board.setPiece({x, y}, new Queen(color));
            } else if (type == Piece::Type::Rook) {
                board.setPiece({x, y}, new Rook(color));
            } else if (type == Piece::Type::Bishop) {
                board.setPiece({x, y}, new Bishop(color));
            } else if (type == Piece::Type::Knight) {
                board.setPiece({x, y}, new Knight(color));
            }
        }

        void switchTurn() {
            board.switchTurn();
        }

        int getPieceType(int x, int y) {
            Piece* piece = board.getPieceFromGrid({x, y});
            if (piece == nullptr) return -1;
            return static_cast<int>(piece->getType());
        }

        int getPieceColor(int x, int y) {
            Piece* piece = board.getPieceFromGrid({x, y});
            if (piece == nullptr) return -1;
            return static_cast<int>(piece->getColor());
        }

        int getCurrentTurn() {
            return static_cast<int>(board.getCurrentTurn());
        }

        bool isKingChecked() {
            return board.isKingChecked();
        }

        bool isCheckmate() {
            return board.isCheckmate();
        }

        bool isStalemate() {
            return board.isStalemate();
        }
};

EMSCRIPTEN_BINDINGS(chess_module) {
    class_<WebGame>("WebGame")
        .constructor<>()
        .function("movePiece", &WebGame::movePiece)
        .function("isValidMove", &WebGame::isValidMove)
        .function("promote", &WebGame::promote)
        .function("switchTurn", &WebGame::switchTurn)
        .function("getPieceType", &WebGame::getPieceType)
        .function("getPieceColor", &WebGame::getPieceColor)
        .function("getCurrentTurn", &WebGame::getCurrentTurn)
        .function("isKingChecked", &WebGame::isKingChecked)
        .function("isCheckmate", &WebGame::isCheckmate)
        .function("isStalemate", &WebGame::isStalemate);
}
