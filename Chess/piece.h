#pragma once

enum class PieceType { None, Pawn, Knight, Bishop, Rook, Queen, King };
enum class Color { None, White, Black };

struct Piece {
    PieceType type;
    Color color;
};
