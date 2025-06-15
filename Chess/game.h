#pragma once
#include <iostream>

#include "board.h"

enum class PlayerColor { None, White, Black };

class Game {
public:
    Game();
    void newGame();
    bool makeMove(int fromX, int fromY, int toX, int toY);
	Board getBoard();
    std::string getCurrentPlayerColor();
    // Add more as needed

private:
    Board board;
    PlayerColor currentTurn;
};
