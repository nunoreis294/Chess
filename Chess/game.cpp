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
}

bool Game::makeMove(int fromX, int fromY, int toX, int toY)
{

}

Board Game::getBoard()
{
	return Game::board;
}