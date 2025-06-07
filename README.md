# Chess

2D

`enum Piece { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum Color { NONE, WHITE, BLACK };

struct Square {
    Piece piece;
    Color color;
};

Square board[8][8];`


Bitboard

'using Bitboard = uint64_t;

Bitboard whitePawns;
Bitboard blackPawns;
// ... one for each piece type and color'

-
 
Suggested Project Structure

Chess/
│
├── main.cpp                // Entry point, initializes GUI and game loop
├── Board.h / Board.cpp     // Board representation and logic (2D array, later bitboard)
├── Piece.h                 // Piece definitions and enums
├── Game.h / Game.cpp       // Game state, move handling, turn management
├── Engine.h / Engine.cpp   // AI/engine logic (bitboard-based, added later)
├── Gui.h / Gui.cpp         // GUI rendering and user interaction (SFML)
└── utils.h / utils.cpp     // Utility functions (optional)

—

Key Abstractions
•	Board: Encapsulates board state and provides methods for querying and updating the board.
•	Game: Manages turns, move validation, and game status.
•	Engine: Handles move generation and evaluation for the computer player.
•	Gui: Handles drawing the board and pieces, and user input.

—

Example: Board Abstraction (2D Array)

board.h

'#pragma once
#include "Piece.h"

class Board {
public:
    Board();
    void reset();
    bool move(int fromX, int fromY, int toX, int toY);
    Piece getPiece(int x, int y) const;
    // Add more as needed

private:
    Piece board[8][8];
};'

—

Example: Piece Definition

piece.h

'#pragma once

enum class PieceType { None, Pawn, Knight, Bishop, Rook, Queen, King };
enum class Color { None, White, Black };

struct Piece {
    PieceType type;
    Color color;
};'

—

Example: Game Management

game.h

'#pragma once
#include "Board.h"

class Game {
public:
    Game();
    void newGame();
    bool makeMove(int fromX, int fromY, int toX, int toY);
    // Add more as needed

private:
    Board board;
    Color currentTurn;
};'

—

Migration to Bitboards
•	When you add the engine, implement a Bitboard class or methods in Board for bitboard representation.
•	Keep the GUI and user interaction using the 2D array, and convert to bitboards for engine calculations.

—

Tips
•	Use clear interfaces in your classes so you can swap out the internal board representation later.
•	Keep GUI code separate from game logic and board representation.
