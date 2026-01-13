#include "board.h"

// Constructor
Board::Board()
{
	reset();
}

// Reset the board to the initial chess position
void Board::reset()
{
	// Clear all squares
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			board[y][x] = { PieceType::None, PieceColor::None };

	// Place pawns
	for (int x = 0; x < 8; x++)
	{
		board[6][x] = { PieceType::Pawn, PieceColor::White };
		board[1][x] = { PieceType::Pawn, PieceColor::Black };
	}

	// Place other pieces
	PieceType backRow[] = {
		PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
		PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook };

	for (int x = 0; x < 8; x++)
	{
		board[7][x] = { backRow[x], PieceColor::White};
		board[0][x] = { backRow[x], PieceColor::Black };
	}
}

// Move a piece from (fromX, fromY) to (toX, toY)
bool Board::move(int fromX, int fromY, int toX, int toY)
{
	return true;
}

// Get the piece at position (x, y)
Piece Board::getPiece(int x, int y) const
{
	return board[y][x];
}

// Check if the king of the given color is in check
bool Board::isKingChecked(PieceColor pieceColor) const
{
	return false;
}

// Get possible squares for the selected piece
std::vector<sf::Vector2f> Board::getPossibleSquares(sf::Vector2f selectedSquare) const
{
	std::vector<sf::Vector2f> possibleSquares;

	return possibleSquares;
}

// Get attacked squares for the selected piece
std::vector<sf::Vector2f> Board::getAttackedSquares(sf::Vector2f selectedSquare) const
{
	std::vector<sf::Vector2f> attackedSquares;

	return attackedSquares;
}