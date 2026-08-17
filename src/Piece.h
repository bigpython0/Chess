#pragma once

#include <cmath>

struct Position {
    int x, y;
};

class Board;

class Piece {
    public:
    enum class Color {White, Black};
    enum class Type {Pawn, Bishop, Knight, Rook, Queen, King};

    protected:
        Type type;
        Color color;

    public:
        Piece(Color c, Type t)
        : color(c), type(t) {}
        virtual ~Piece() = default;

        Piece::Type getType() {
            return type;
        }

        virtual bool isValidMove(Position from, Position to, Board& board) {
            return true;
        }

        Piece::Color getColor() {
            return this->color;
        }
};

class Pawn : public Piece {
    private:
        bool isFirstMove = true;

    public:

        explicit Pawn(Color c)
        : Piece(c, Type::Pawn) {}

        bool isValidMove(Position from, Position to, Board &board) override;

        void markMove() {
            isFirstMove = false;
        }
};

class Knight : public Piece {
    public:
        explicit Knight(Color c)
        : Piece(c, Type::Knight) {}

        bool isValidMove(Position from, Position to, Board &board) override;
};

class Bishop : public Piece {
    public:
        explicit Bishop(Color c)
        : Piece(c, Type::Bishop) {}

        bool isValidMove(Position from, Position to, Board &board) override;
};

class Queen : public Piece {
    public:
        explicit Queen(Color c)
        : Piece(c, Type::Queen) {}

        bool isValidMove(Position from, Position to, Board &board) override;
};

class King : public Piece {
    private:
        bool isFirstMove = true;

    public:
        bool isChecked;

        explicit King(Color c)
        : Piece(c, Type::King) {
            isChecked = false;
        }

        bool isValidMove(Position from, Position to, Board &board) override;

        bool checkIfFirstMove() {
            return isFirstMove;
        }

        void markMove() {
            isFirstMove = false;
        }
};

class Rook : public Piece {
    private:
        bool isFirstMove = true;

    public:
        explicit Rook(Color c)
        : Piece(c, Type::Rook)
        {}

        bool isValidMove(Position from, Position to, Board &board) override;

        bool checkIfFirstMove() {
            return isFirstMove;
        }

        void markMove() {
            isFirstMove = false;
        }
};
