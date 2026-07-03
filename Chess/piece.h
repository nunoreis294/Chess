#pragma once

/**
 * @file piece.h
 * @brief Piece type and color definitions used across the project.
 */

/** Piece types used on the chessboard. */
enum class PieceType { None, Pawn, Knight, Bishop, Rook, Queen, King };

/** Piece color (side). */
enum class PieceColor { None, White, Black };

/** Simple piece structure combining type and color. */
struct Piece {
    PieceType type;
    PieceColor color;
};
