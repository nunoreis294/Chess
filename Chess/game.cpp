#include "game.h"

// Constructor
Game::Game()
{
	newGame();
}

// Start a new game
void Game::newGame()
{
	board.reset();
	currentPlayerColor = PlayerColor::White;
}

// Make a move from (fromX, fromY) to (toX, toY)
bool Game::makeMove(int fromX, int fromY, int toX, int toY)
{
	return true;
}

// Get the current board state
Board Game::getBoard()
{
	return Game::board;
}

// Get the current player's color as a string
std::string Game::getCurrentPlayerColor()
{
	if (currentPlayerColor == PlayerColor::White)
		return "White";
	else
		return "Black";
}