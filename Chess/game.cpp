#include "game.h"

// Constructor: initialize the window and any resources
Game::Game()
{
	newGame();
}

// Simple move logic: does not check for legality yet
void Game::newGame()
{
	board.reset();
	currentPlayerColor = PlayerColor::Black;
}

bool Game::makeMove(int fromX, int fromY, int toX, int toY)
{
	return true;
}

Board Game::getBoard()
{
	return Game::board;
}

std::string Game::getCurrentPlayerColor()
{
	if (currentPlayerColor == PlayerColor::White)
		return "White";
	else
		return "Black";
}