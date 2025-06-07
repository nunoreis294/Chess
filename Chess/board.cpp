#include "board.h"

// Constructor: initialize the window and any resources
Board::Board()
{
	reset();
}

// Set up the initial chess position
void Board::reset()
{
	// Clear all squares
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			board[y][x] = { PieceType::None, Color::None };

	// Place pawns
	for (int x = 0; x < 8; x++)
	{
		board[1][x] = { PieceType::Pawn, Color::White };
		board[6][x] = { PieceType::Pawn, Color::Black };
	}

	// Place other pieces
	PieceType backRow[] = {
		PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
		PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook };

	for (int x = 0; x < 8; x++)
	{
		board[0][x] = { backRow[x], Color::White};
		board[7][x] = { backRow[x], Color::Black };
	}
}

bool Board::move(int fromX, int fromY, int toX, int toY)
{

}

Piece Board::getPiece(int x, int y) const
{
	return board[y][x];
}
