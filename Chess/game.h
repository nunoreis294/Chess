#pragma once
#include <iostream>

#include "board.h"

/**
 * @file game.h
 * @brief High-level game management: turn handling and move execution.
 */

/** Represents the current player. */
enum class PlayerColor { None, White, Black };

/**
 * @brief Game controller that owns a Board and manages turns and moves.
 */
class Game {
public:
    Game();

    /** Start a new game and reset internal state. */
    void newGame();

    /** Attempt to make a move; returns true on success. Coordinates are 0-based array indices. */
    bool makeMove(int fromX, int fromY, int toX, int toY);

    /** Get a pointer to the internal Board. */
    Board* getBoard();

    /** Get current player color. */
    PlayerColor getCurrentPlayerColor();

    /** Swap current player to the other side. */
    void changePlayerColor();

    /** Set a piece at given coordinates (utility). */
    void setPiece(int x, int y, Piece piece);

private:
    Board board;
    PlayerColor currentPlayerColor;
};
