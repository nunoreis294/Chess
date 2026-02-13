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
	sf::Vector2i selectedPiece = sf::Vector2i(fromX, fromY);
	sf::Vector2i selectedSquare = sf::Vector2i(toX, toY);

	std::vector<sf::Vector2i> possibleSquares = { board.getPossibleSquares(selectedPiece) };

	std::vector<sf::Vector2i> attackedSquares = { board.getAttackedSquares(selectedPiece) };

	possibleSquares.insert(possibleSquares.end(), attackedSquares.begin(), attackedSquares.end());

	//std::cout << "move fromX " << (int)selectedPiece.x << ", fromY " << (int)selectedPiece.y << " - toX " << (int)selectedSquare.x << ", toY " << (int)selectedSquare.y << std::endl;


	for (sf::Vector2i possibleSquare : possibleSquares)
	{
		if (possibleSquare.x == selectedSquare.x && possibleSquare.y == selectedSquare.y)
		{
			//std::cout << "move fromX " << (int)selectedPiece.x << ", fromY " << (int)selectedPiece.y << " - toX " << (int)selectedSquare.x << ", toY " << (int)selectedSquare.y << std::endl;

			return board.move((int)selectedPiece.x - 1, (int)selectedPiece.y - 1, (int)selectedSquare.x - 1, (int)selectedSquare.y - 1);
		}
	}

	return false;
}

// Get the current board state
Board* Game::getBoard()
{
	return &board;
}

// Get the current player's color as a string
std::string Game::getCurrentPlayerColor()
{
	if (currentPlayerColor == PlayerColor::White)
		return "White";
	else
		return "Black";
}

// Change player color
void Game::changePlayerColor()
{
	if (currentPlayerColor == PlayerColor::White)
		currentPlayerColor = PlayerColor::Black;
	else
		currentPlayerColor = PlayerColor::White;

	return;
}