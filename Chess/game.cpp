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
	currentTurn = PlayerColor::White;
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
	if (currentTurn == PlayerColor::White)
		return "White";
	else
		return "Black";
}