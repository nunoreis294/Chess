#pragma once
#include "board.h"

class Game {
public:
    Game();
    void newGame();
    bool makeMove(int fromX, int fromY, int toX, int toY);
	Board getBoard();
    // Add more as needed

private:
    Board board;
    Color currentTurn;
};
