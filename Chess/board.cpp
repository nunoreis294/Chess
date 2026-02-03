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

	if (selectedSquare.x < 1 || selectedSquare.x >= 9 || selectedSquare.y < 1 || selectedSquare.y >= 9)
		return possibleSquares;
	else
	{
		if (board[(int)selectedSquare.y - 1][(int)selectedSquare.x - 1].type == PieceType::Pawn)
		{
			int direction = board[(int)selectedSquare.y - 1][(int)selectedSquare.x - 1].color == PieceColor::White ? -1 : 1;
			sf::Vector2f forwardSquare = sf::Vector2f(selectedSquare.x, selectedSquare.y + direction);
			if (forwardSquare.y >= 1 && forwardSquare.y <= 8 && board[(int)forwardSquare.y - 1][(int)forwardSquare.x - 1].type == PieceType::None)
			{
				possibleSquares.push_back(forwardSquare);
				// Check for initial double move
				if ((board[(int)selectedSquare.y - 1][(int)selectedSquare.x - 1].color == PieceColor::White && selectedSquare.y == 7) ||
					(board[(int)selectedSquare.y - 1][(int)selectedSquare.x - 1].color == PieceColor::Black && selectedSquare.y == 2))
				{
					sf::Vector2f doubleForwardSquare = sf::Vector2f(selectedSquare.x, selectedSquare.y + 2 * direction);
					if (board[(int)doubleForwardSquare.y - 1][(int)doubleForwardSquare.x - 1].type == PieceType::None)
					{
						possibleSquares.push_back(doubleForwardSquare);
					}
				}
			}
			// Check for captures
			sf::Vector2f captureLeft = sf::Vector2f(selectedSquare.x - 1, selectedSquare.y + direction);
			sf::Vector2f captureRight = sf::Vector2f(selectedSquare.x + 1, selectedSquare.y + direction);
			if (captureLeft.x >= 1 && captureLeft.x <= 8 && captureLeft.y >= 1 && captureLeft.y <= 8)
			{
				Piece targetPiece = board[(int)captureLeft.y - 1][(int)captureLeft.x - 1];
				if (targetPiece.type != PieceType::None && targetPiece.color != board[(int)selectedSquare.y - 1][(int)selectedSquare.x - 1].color)
				{
					possibleSquares.push_back(captureLeft);
				}
			}
		}
	}

	return possibleSquares;
}

// Get attacked squares for the selected piece
std::vector<sf::Vector2f> Board::getAttackedSquares(sf::Vector2f selectedSquare) const
{
	std::vector<sf::Vector2f> attackedSquares;

	return attackedSquares;
}