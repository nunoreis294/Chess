#pragma once
#include <iostream>

#include "board.h"

enum class PlayerColor { None, White, Black };

class Game {
public:
    Game();
    void newGame();
    bool makeMove(int fromX, int fromY, int toX, int toY);
	Board* getBoard();
    PlayerColor getCurrentPlayerColor();
    void changePlayerColor();
    void setPiece(int x, int y, Piece piece);

private:
    Board board;
    PlayerColor currentPlayerColor;
};
