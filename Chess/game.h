#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "board.h"

/**
 * @file game.h
 * @brief High-level game management: turn handling and move execution.
 */

/** Represents the current player. */
enum class PlayerColor { None, White, Black };

enum class GameResultType { None, Checkmate, Draw, Resignation };

struct GameResult {
    GameResultType type;
    PieceColor winner;
    std::string message;
};

struct GameSnapshot {
    Board board;
    PlayerColor currentPlayerColor;
    GameResult gameResult;
};

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

    /** Evaluate whether the current side to play is checkmated or in a draw. */
    void updateGameState();

    /** End the game by resignation for the current player. */
    void resignCurrentPlayer();

    /** Whether the game has already ended. */
    bool isGameOver() const;

    /** Save the current board state to the history for later review. */
    void saveCurrentState();

    /** Navigate to the previous stored position in the current game history. */
    void goToPreviousMove();

    /** Navigate to the next stored position in the current game history. */
    void goToNextMove();

    /** Whether a previous position is available for analysis. */
    bool canGoToPreviousMove() const;

    /** Whether a following position is available for analysis. */
    bool canGoToNextMove() const;

    /** Get the current game result. */
    GameResult getGameResult() const;

    /** Set a piece at given coordinates (utility). */
    void setPiece(int x, int y, Piece piece);

private:
    Board board;
    PlayerColor currentPlayerColor;
    GameResult gameResult;
    std::vector<GameSnapshot> history;
    int historyIndex;

    void restoreStateAt(int index);
};
