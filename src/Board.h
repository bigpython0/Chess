#pragma once

#include <iostream>
#include <cmath>
#include <map>
#include "Piece.h"

class Board {
    private:
        Piece* board[8][8] = { nullptr };
        Position whiteKingPos;
        Position blackKingPos;
        bool gameOver = false;
        Piece::Color currentTurn = Piece::Color::White;

    public:

        Board() {
            //WHITE ____________________
            for(int x = 0; x<8; x++) {
                setPiece({x,1}, new Pawn(Piece::Color::White));
            }
            //alle Pieces
            setPiece({1,0}, new Knight(Piece::Color::White));
            setPiece({6,0}, new Knight(Piece::Color::White));

            setPiece({2,0}, new Bishop(Piece::Color::White));
            setPiece({5,0}, new Bishop(Piece::Color::White));

            setPiece({0,0}, new Rook(Piece::Color::White));
            setPiece({7,0}, new Rook(Piece::Color::White));

            setPiece({3,0}, new Queen(Piece::Color::White));
            setPiece({4,0}, new King(Piece::Color::White));
            whiteKingPos = {4,0};




            //BLACK____________________
            for(int x = 0; x<8; x++) {
                setPiece({x,6}, new Pawn(Piece::Color::Black));
            }
            setPiece({1,7}, new Knight(Piece::Color::Black));
            setPiece({6,7}, new Knight(Piece::Color::Black));

            setPiece({2,7}, new Bishop(Piece::Color::Black));
            setPiece({5,7}, new Bishop(Piece::Color::Black));

            setPiece({0,7}, new Rook(Piece::Color::Black));
            setPiece({7,7}, new Rook(Piece::Color::Black));

            setPiece({3,7}, new Queen(Piece::Color::Black));
            setPiece({4,7}, new King(Piece::Color::Black));
            blackKingPos = {4,7};
        }

        bool isGameOver() {
            return gameOver;
        }

        bool canPlayerMove() {
            for(int y = 0; y < 8; y++) { //from = y,x
                for(int x = 0; x < 8; x++) {
                    Piece* piece = board[y][x];
                    if(piece == nullptr || piece->getColor() != currentTurn) continue;

                    for(int toY = 0; toY<8; toY++) { //to = i,k
                        for(int toX = 0; toX<8; toX++) {
                            if(piece->isValidMove({x,y}, {toX, toY}, *this)) {
                                Piece* capturedPiece = board[toY][toX];

                                Position oldKingPos = (currentTurn == Piece::Color::White) ? whiteKingPos : blackKingPos;

                                board[toY][toX] = piece;
                                board[y][x] = nullptr;

                                if(piece->getType() == Piece::Type::King) {
                                    (currentTurn == Piece::Color::White) ? (whiteKingPos = {toX, toY}) : (blackKingPos = {toX,toY});
                                }

                                if(!isKingChecked()) {
                                    board[toY][toX] = capturedPiece;
                                    board[y][x] = piece;
                                    if(piece->getType() == Piece::Type::King) {
                                        (currentTurn == Piece::Color::White) ? (whiteKingPos = oldKingPos) : (blackKingPos = oldKingPos);
                                    }
                                    return true;
                                } else { //undo move
                                    board[toY][toX] = capturedPiece;
                                    board[y][x] = piece;
                                    if(piece->getType() == Piece::Type::King) {
                                        (currentTurn == Piece::Color::White) ? (whiteKingPos = oldKingPos) : (blackKingPos = oldKingPos);
                                    }
                                }
                            }
                        }
                    }

                }
            }
            return false;
        }

        bool isCheckmate() {
            if(isKingChecked() && !canPlayerMove()) {
                return true;
            }

            return false;
        }

        bool isStalemate() {
            if(!isKingChecked() && !canPlayerMove()) {
                return true;
            }

            return false;
        }

        Piece::Color getCurrentTurn() {
            return currentTurn;
        }

        void switchTurn() {
            if(currentTurn == Piece::Color::White) {
                currentTurn = Piece::Color::Black;
            } else {
                currentTurn = Piece::Color::White;
            }
        }

        void printBoard() { //how its actually STORED, NOT how its RENDERED
            std::cout << "  0 1 2 3 4 5 6 7" << std::endl; // Spalten-Indizes
            static std::map<Piece::Type, char> symbols = {
                {Piece::Type::King, 'K'}, {Piece::Type::Queen, 'Q'}, {Piece::Type::Knight, 'N'},
                {Piece::Type::Bishop, 'B'}, {Piece::Type::Rook, 'R'}, {Piece::Type::Pawn, 'P'}
            };

            for (int y = 0; y < 8; y++) {
                std::cout << y << " "; // Zeilen-Index
                for (int x = 0; x < 8; x++) {
                    if (board[y][x] == nullptr) {
                        std::cout << ". "; // Leeres Feld
                    } else {
                        std::cout << symbols[board[y][x]->getType()] << " "; // 'P' für Piece (irgendeine Figur)
                    }
                }
                std::cout << std::endl;
            }
            std::cout << "---------------" << std::endl;
        }

        void setPiece(Position pos, Piece* piece) {
            if(pos.x >=0 && pos.y>=0 && pos.x<8 && pos.y <8){
                if(board[pos.y][pos.x] != nullptr){
                delete board[pos.y][pos.x];
            }
                board[pos.y][pos.x] = piece;
            }
        }

        King* getKing() { //gives the King who's move it would be --> currentTurn King
            if(currentTurn == Piece::Color::White) {
                return dynamic_cast<King*>(board[whiteKingPos.y][whiteKingPos.x]);
            } else {
                return dynamic_cast<King*>(board[blackKingPos.y][blackKingPos.x]);
            }
        }

        bool movePiece(Position from, Position to) {
            Piece* currentPiece = board[from.y][from.x];

            if(currentPiece == nullptr || currentPiece->getColor() != currentTurn) return false;

            if(currentPiece->isValidMove(from,to, *this)){
                Piece* capturedPiece = board[to.y][to.x];

                Position oldKingPos = (currentTurn == Piece::Color::White) ? whiteKingPos : blackKingPos;

                board[to.y][to.x] = currentPiece;
                board[from.y][from.x] = nullptr;

                //in case of castling
                bool isMoveCastle = false;
                int rookX;
                int direction;

                if(currentPiece->getType() == Piece::Type::King) {
                    (currentTurn == Piece::Color::White) ? (whiteKingPos = to) : (blackKingPos = to);

                    if((std::abs(to.x - from.x) == 2) && !isKingChecked()) {
                        isMoveCastle = true;
                        rookX = (to.x - from.x < 0) ? 0 : 7;
                        direction = (rookX == 0) ? -1 : 1;
                        board[from.y][from.x + direction] = board[from.y][rookX];
                        board[from.y][rookX] = nullptr;
                    }
                }

                if(isKingChecked()) {
                    //move invalid
                    if(isMoveCastle){
                        board[from.y][rookX] = board[from.y][from.x + direction];
                        board[from.y][from.x + direction] = nullptr;
                    }

                    board[from.y][from.x] = currentPiece;
                    board[to.y][to.x] = capturedPiece;

                    if(currentPiece->getType() == Piece::Type::King) {
                        (currentTurn == Piece::Color::White) ? (whiteKingPos = oldKingPos) : (blackKingPos = oldKingPos);
                    }
                    return false;
                }

                if(currentPiece->getType() == Piece::Type::Pawn) {
                    Pawn* pawn = dynamic_cast<Pawn*>(currentPiece);
                    pawn->markMove();
                }

                if(currentPiece->getType() == Piece::Type::King) {
                        King* king = dynamic_cast<King*>(currentPiece);
                        king->markMove();
                }

                if(capturedPiece != nullptr) {
                    delete capturedPiece; //free up space from dead piece
                }

                return true;
            } else {
                return false;
            }
        }

        Piece* getPieceFromGrid(Position pos) { //input is board pos, already converted
            return board[pos.y][pos.x];
        }

        bool isPathBlocked(Position from, Position to) { //PATH BLOCKED = TRUE
            int xDiff = to.x - from.x;
            int yDiff = to.y - from.y;

            int stepX = (xDiff == 0) ? 0 : (xDiff > 0 ? 1 : -1);
            int stepY = (yDiff == 0) ? 0 : (yDiff > 0 ? 1 : -1);

            int currX = from.x + stepX;
            int currY = from.y + stepY;

            while(currX != to.x || currY != to.y) {
                if(board[currY][currX] != nullptr) {
                    return true;
                }

                currX += stepX;
                currY += stepY;
            }
            return false;
        }

        bool isSquareInCheck(Position square) {
        //Rook / Queen steps
            //X AXIS
            for(int direction = -1; direction <=2; direction += 2) {
                for(int i = square.x + direction; 0<=i && i<= 7; i+=(direction)*(1)) {
                    Piece* foundPiece = board[square.y][i];
                    if(foundPiece != nullptr) {
                        if(foundPiece->getType() == Piece::Type::Queen || foundPiece->getType() == Piece::Type::Rook) {
                            if(foundPiece->getColor() != currentTurn) {
                                return true;
                            }
                        }
                        break;
                    }
                }
            }

            //Y AXIS
            for(int direction = -1; direction <=2; direction += 2) {
                for(int i = square.y + direction;0<=i && i<= 7; i+=(direction)*(1)) {
                    Piece* foundPiece = board[i][square.x];
                    if(foundPiece != nullptr) {
                        if(foundPiece->getType() == Piece::Type::Queen || foundPiece->getType() == Piece::Type::Rook) {
                            if(foundPiece->getColor() != currentTurn) {
                                return true;
                            }
                        }
                        break;
                    }
                }
            }

        //bishop / queen // DIAGONAL
        for(int dirX : {-1, 1}) {
            for(int dirY : {-1,1}) {
                for(int dist = 1; dist <8 ; dist++) {
                    int tx = square.x + (dirX * dist);
                    int ty = square.y + (dirY*dist);

                    if(tx < 0 || tx > 7 || ty < 0 || ty > 7) break;

                    Piece* foundPiece = board[ty][tx];
                    if(foundPiece != nullptr) {
                        if(foundPiece->getType() == Piece::Type::Queen || foundPiece->getType() == Piece::Type::Bishop) {
                            if(foundPiece->getColor() != currentTurn) {
                                return true;
                            }
                        }
                        break;
                    }
                }
            }
        }
        //knight
        Position knightMoves[] = {
            {2,1}, {2,-1}, {-2,1}, {-2,-1},
            {1,2}, {1,-2}, {-1,2}, {-1,-2}
        };

        for(const auto& move : knightMoves) {
            int x = square.x + move.x;
            int y = square.y + move.y;
            if( x < 0 || x > 7 || y < 0 || y > 7) continue;
            Piece* foundPiece = board[y][x];
            if(foundPiece != nullptr && foundPiece->getType() == Piece::Type::Knight && foundPiece->getColor() != currentTurn) {
                return true;
            }
        }

        //PAWN
        int direction = currentTurn == Piece::Color::White ? 1 : -1;
        for(int i : {-1, 1}) {
            int targetY = square.y + (direction * 1);
            int targetX = square.x + i;
            if(targetY < 0 || targetY > 7 || targetX < 0 || targetX > 7) continue;
            Piece* potentialPawn = board[targetY][targetX];
            if(potentialPawn != nullptr && potentialPawn->getType() == Piece::Type::Pawn
                && potentialPawn->getColor()!=currentTurn) {
                return true;
            }
        }

        return false;
        }

        bool isKingChecked(){
            Piece* currentKing = getKing();
            Position KingPos;
            if(currentKing->getColor() == Piece::Color::White) {
                KingPos = whiteKingPos;
            } else {
                KingPos = blackKingPos;
            }

            if(isSquareInCheck(KingPos)) {
                return true;
            }
            return false;
        }
};


bool Pawn::isValidMove(Position from, Position to, Board& board){
        int direction = (color == Piece::Color::White) ? 1 : -1;
        int yDiff = to.y - from.y;
        int xDiff = from.x-to.x;

        //normal move / first move
        if (xDiff == 0) {
            if(isFirstMove) {
                if(board.getPieceFromGrid(to) == nullptr && !(board.isPathBlocked(from, to))){
                    if(yDiff==2*direction || yDiff == 1 * direction) {
                        return true;
                        std::cout << "is valid move";
                    }
                }
            } else if (yDiff == (1*direction) && board.getPieceFromGrid(to) == nullptr){
                return true;
            }
            //attack
        }
        else if (std::abs(xDiff) == 1 && yDiff == direction) {
            Piece* target = board.getPieceFromGrid(to);
            // Es MUSS ein Gegner dort stehen
            if (target != nullptr && target->getColor() != this->color) {
                isFirstMove = false;
                return true;
            }
        }



        return false;
    };

bool Knight::isValidMove(Position from, Position to, Board& board){
    int xDiff = to.x - from.x;
    int yDiff = to.y - from.y;

    //safety from nullptr exception
    if(to.x < 0 || to.x > 7 || from.y < 0 || from.y > 7) {
        return false;
    }

    Piece* piece = board.getPieceFromGrid(to);
    if( piece != nullptr && piece->getColor() == this->color) {
        return false;
    }

    if((std::abs(xDiff) == 1 && std::abs(yDiff) == 2 ) || std::abs(xDiff) == 2 && std::abs(yDiff) == 1 ) {
        return true;
    }

    return false;


}

bool Bishop::isValidMove(Position from, Position to, Board& board){
    int xDiff = to.x - from.x;
    int yDiff = to.y - from.y;

    if(!(std::abs(xDiff) == std::abs(yDiff))) {
        return false;
    }

    Piece* targetPiece = board.getPieceFromGrid(to);
    if( !board.isPathBlocked(from, to)) {
        return (targetPiece != nullptr) ? targetPiece->getColor() != color : true;
    }
    return false;
}
bool Queen::isValidMove(Position from, Position to, Board& board){
    int xDiff = to.x - from.x;
    int yDiff = to.y - from.y;

    if(((xDiff == 0 && std::abs(yDiff) > 0 )|| (yDiff == 0 && std::abs(xDiff) > 0))) {
            if( !board.isPathBlocked(from, to)) {
                Piece* targetPiece = board.getPieceFromGrid(to);
                return (targetPiece != nullptr) ? targetPiece->getColor() != color : true;
            }
    }

    if(!(std::abs(xDiff) == std::abs(yDiff))) {
        return false;
    }

    Piece* targetPiece = board.getPieceFromGrid(to);
    if( !board.isPathBlocked(from, to)) {
        return (targetPiece != nullptr) ? targetPiece->getColor() != color : true;
    }
    return false;
}

bool King::isValidMove(Position from, Position to, Board& board){
        int xDiff = to.x - from.x;
        xDiff = std::abs(xDiff);
        int yDiff = to.y - from.y;
        yDiff = std::abs(yDiff);

        Piece* piece = board.getPieceFromGrid(to);
            if( piece != nullptr && piece->getColor() == this->color) {
                return false;
            }

        if(( xDiff == 1 || yDiff == 1 || xDiff + yDiff == 2 )&& xDiff <2 && yDiff < 2) {
            return true;
        } else if(isFirstMove && !board.isPathBlocked(from, to) && yDiff == 0 && xDiff == 2) {
                return true;
        }
        return false;
}

bool Rook::isValidMove(Position from, Position to, Board& board){
        int xDiff = to.x - from.x;
        int yDiff = to.y - from.y;

        if(((xDiff == 0 && std::abs(yDiff) > 0 )|| (yDiff == 0 && std::abs(xDiff) > 0))) {
                if( !board.isPathBlocked(from, to)) {
                    Piece* targetPiece = board.getPieceFromGrid(to);
                    return (targetPiece != nullptr) ? targetPiece->getColor() != color : true;
                }
        }
        return false;

    }
