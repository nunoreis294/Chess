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
	gameResult = { GameResultType::None, PieceColor::None, "" };
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
PlayerColor Game::getCurrentPlayerColor()
{
	return currentPlayerColor;
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

void Game::updateGameState()
{
	PieceColor sideToMove = currentPlayerColor == PlayerColor::White ? PieceColor::White : PieceColor::Black;

	if (board.isKingCheckMated(sideToMove))
	{
		PieceColor winner = sideToMove == PieceColor::White ? PieceColor::Black : PieceColor::White;
		std::string message = winner == PieceColor::White ? "White wins" : "Black wins";
		gameResult = { GameResultType::Checkmate, winner, message };
	}
	else if (board.isStalemated(sideToMove) || board.isInsufficientMaterial())
	{
		gameResult = { GameResultType::Draw, PieceColor::None, "Draw" };
	}
	else
	{
		gameResult = { GameResultType::None, PieceColor::None, "" };
	}
}

void Game::resignCurrentPlayer()
{
	PieceColor winner = currentPlayerColor == PlayerColor::White ? PieceColor::Black : PieceColor::White;
	std::string message = winner == PieceColor::White ? "White wins by resignation" : "Black wins by resignation";
	gameResult = { GameResultType::Resignation, winner, message };
}

bool Game::isGameOver() const
{
	return gameResult.type != GameResultType::None;
}

GameResult Game::getGameResult() const
{
	return gameResult;
}

void Game::setPiece(int x, int y, Piece piece)
{
	board.setPiece(x, y, piece);

	return;
}
