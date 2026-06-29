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

	board[2][2] = { PieceType::Pawn, PieceColor::White };
}

// Move a piece from (fromX, fromY) to (toX, toY)
bool Board::move(int fromX, int fromY, int toX, int toY)
{
	PieceType pieceType = board[fromY][fromX].type;
	PieceColor pieceColor = board[fromY][fromX].color;

	board[toY][toX] = { pieceType, pieceColor };
	board[fromY][fromX] = { PieceType::None, PieceColor::None };

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
std::vector<sf::Vector2i> Board::getPossibleSquares(sf::Vector2i selectedPiece) const
{
	std::vector<sf::Vector2i> possibleSquares;

	Piece piece = getPiece((int)selectedPiece.x - 1, (int)selectedPiece.y - 1);

	if (selectedPiece.x != -1.f && selectedPiece.y != -1.f)
	{
		switch (piece.type)
		{
		case PieceType::Pawn: {
			int direction = piece.color == PieceColor::White ? -1 : 1;
			sf::Vector2i forwardSquare = sf::Vector2i(selectedPiece.x, selectedPiece.y + direction);

			if (forwardSquare.y >= 1 && forwardSquare.y <= 8 && board[(int)forwardSquare.y - 1][(int)forwardSquare.x - 1].type == PieceType::None)
			{
				possibleSquares.push_back(forwardSquare);

				// Check for initial double move
				if ((piece.color == PieceColor::White && selectedPiece.y == 7) ||
					(piece.color == PieceColor::Black && selectedPiece.y == 2))
				{
					sf::Vector2i doubleForwardSquare = sf::Vector2i(selectedPiece.x, selectedPiece.y + 2 * direction);

					if (board[(int)doubleForwardSquare.y - 1][(int)doubleForwardSquare.x - 1].type == PieceType::None)
					{
						possibleSquares.push_back(doubleForwardSquare);
					}
				}
			}
		}
			break;
		case PieceType::Knight: {
			int knightMoves[8][2] = {
				{ 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 },
				{ -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 }
			};

			for (const auto& move : knightMoves)
			{
				sf::Vector2i targetSquare = sf::Vector2i(selectedPiece.x + move[0], selectedPiece.y + move[1]);

				if (targetSquare.x >= 1 && targetSquare.x <= 8 && targetSquare.y >= 1 && targetSquare.y <= 8)
				{
					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type == PieceType::None)
					{
						possibleSquares.push_back(targetSquare);
					}
				}
			}
		}
			break;
		case PieceType::Bishop: {
			int directions[4][2] = {
				{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
			};

			for (const auto& dir : directions)
			{
				sf::Vector2i targetSquare = selectedPiece;

				while (true)
				{
					targetSquare.x += dir[0];
					targetSquare.y += dir[1];

					if (targetSquare.x < 1 || targetSquare.x > 8 || targetSquare.y < 1 || targetSquare.y > 8)
						break;

					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type == PieceType::None)
					{
						possibleSquares.push_back(targetSquare);
					}
					else
					{
						break; // Stop if a piece is encountered
					}
				}
			}
		}
			break;
		case PieceType::Rook: {
			int directions[4][2] = {
				{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
			};

			for (const auto& dir : directions)
			{
				sf::Vector2i targetSquare = selectedPiece;

				while (true)
				{
					targetSquare.x += dir[0];
					targetSquare.y += dir[1];

					if (targetSquare.x < 1 || targetSquare.x > 8 || targetSquare.y < 1 || targetSquare.y > 8)
						break;

					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type == PieceType::None)
					{
						possibleSquares.push_back(targetSquare);
					}
					else
					{
						break; // Stop if a piece is encountered
					}
				}
			}
		}
			break;
		case PieceType::Queen: {
			int directions[8][2] = {
				{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
				{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
			};

			for (const auto& dir : directions)
			{
				sf::Vector2i targetSquare = selectedPiece;

				while (true)
				{
					targetSquare.x += dir[0];
					targetSquare.y += dir[1];

					if (targetSquare.x < 1 || targetSquare.x > 8 || targetSquare.y < 1 || targetSquare.y > 8)
						break;

					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type == PieceType::None)
					{
						possibleSquares.push_back(targetSquare);
					}
					else
					{
						break; // Stop if a piece is encountered
					}
				}
			}
		}
			break;
		case PieceType::King: {
			int directions[8][2] = {
				{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
				{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
			};

			for (const auto& dir : directions)
			{
				sf::Vector2i targetSquare = sf::Vector2i(selectedPiece.x + dir[0], selectedPiece.y + dir[1]);

				if (targetSquare.x >= 1 && targetSquare.x <= 8 && targetSquare.y >= 1 && targetSquare.y <= 8)
				{
					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type == PieceType::None)
					{
						possibleSquares.push_back(targetSquare);
					}
				}
			}

			//Castling
		}
			break;
		default:
			break;
		}
	}

	return possibleSquares;
}

// Get attacked squares for the selected piece
std::vector<sf::Vector2i> Board::getAttackedSquares(sf::Vector2i selectedPiece) const
{
	std::vector<sf::Vector2i> attackedSquares;

	Piece piece = getPiece((int)selectedPiece.x - 1, (int)selectedPiece.y - 1);

	if (selectedPiece.x != -1.f && selectedPiece.y != -1.f)
	{
		switch (piece.type)
		{
		case PieceType::Pawn: {
			int direction = piece.color == PieceColor::White ? -1 : 1;

			// Check for captures
			sf::Vector2i captureLeft = sf::Vector2i(selectedPiece.x - 1, selectedPiece.y + direction);
			sf::Vector2i captureRight = sf::Vector2i(selectedPiece.x + 1, selectedPiece.y + direction);

			if (captureLeft.x >= 1 && captureLeft.x <= 8 && captureLeft.y >= 1 && captureLeft.y <= 8)
			{
				Piece targetPiece = board[(int)captureLeft.y - 1][(int)captureLeft.x - 1];

				if (targetPiece.type != PieceType::None && targetPiece.color != board[(int)selectedPiece.y - 1][(int)selectedPiece.x - 1].color)
				{
					attackedSquares.push_back(captureLeft);
				}
			}

			if (captureRight.x >= 1 && captureRight.x <= 8 && captureRight.y >= 1 && captureRight.y <= 8)
			{
				Piece targetPiece = board[(int)captureRight.y - 1][(int)captureRight.x - 1];

				if (targetPiece.type != PieceType::None && targetPiece.color != board[(int)selectedPiece.y - 1][(int)selectedPiece.x - 1].color)
				{
					attackedSquares.push_back(captureRight);
				}
			}

			//en passant
		}
			break;
		case PieceType::Knight: {
			int knightMoves[8][2] = {
				{ 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 },
				{ -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 }
			};

			for (const auto& move : knightMoves)
			{
				sf::Vector2i targetSquare = sf::Vector2i(selectedPiece.x + move[0], selectedPiece.y + move[1]);

				if (targetSquare.x >= 1 && targetSquare.x <= 8 && targetSquare.y >= 1 && targetSquare.y <= 8)
				{
					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type != PieceType::None && targetPiece.color != board[(int)selectedPiece.y - 1][(int)selectedPiece.x - 1].color)
					{
						attackedSquares.push_back(targetSquare);
					}
				}
			}
		}
			break;
		case PieceType::Bishop: {
			int directions[4][2] = {
				{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
			};

			for (const auto& dir : directions)
			{
				sf::Vector2i targetSquare = selectedPiece;

				while (true)
				{
					targetSquare.x += dir[0];
					targetSquare.y += dir[1];

					if (targetSquare.x < 1 || targetSquare.x > 8 || targetSquare.y < 1 || targetSquare.y > 8)
						break;

					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type != PieceType::None && targetPiece.color != board[(int)selectedPiece.y - 1][(int)selectedPiece.x - 1].color)
					{
						attackedSquares.push_back(targetSquare);

						break; // Stop if a piece is encountered
					}
					else if (targetPiece.type != PieceType::None)
					{
						break; // Stop if a piece of the same color is encountered
					}
				}
			}
		}
			break;
		case PieceType::Rook: {
			int directions[4][2] = {
				{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
			};

			for (const auto& dir : directions)
			{
				sf::Vector2i targetSquare = selectedPiece;

				while (true)
				{
					targetSquare.x += dir[0];
					targetSquare.y += dir[1];

					if (targetSquare.x < 1 || targetSquare.x > 8 || targetSquare.y < 1 || targetSquare.y > 8)
						break;

					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type != PieceType::None && targetPiece.color != board[(int)selectedPiece.y - 1][(int)selectedPiece.x - 1].color)
					{
						attackedSquares.push_back(targetSquare);

						break; // Stop if a piece is encountered
					}
					else if (targetPiece.type != PieceType::None)
					{
						break; // Stop if a piece of the same color is encountered
					}
				}
			}
		}
			break;
		case PieceType::Queen: {
			int directions[8][2] = {
				{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
				{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
			};
			for (const auto& dir : directions)
			{
				sf::Vector2i targetSquare = selectedPiece;

				while (true)
				{
					targetSquare.x += dir[0];
					targetSquare.y += dir[1];

					if (targetSquare.x < 1 || targetSquare.x > 8 || targetSquare.y < 1 || targetSquare.y > 8)
						break;

					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type != PieceType::None && targetPiece.color != board[(int)selectedPiece.y - 1][(int)selectedPiece.x - 1].color)
					{
						attackedSquares.push_back(targetSquare);

						break; // Stop if a piece is encountered
					}
					else if (targetPiece.type != PieceType::None)
					{
						break; // Stop if a piece of the same color is encountered
					}
				}
			}
		}
			break;
		case PieceType::King: {
			//ckeck for check
			int directions[8][2] = {
				{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
				{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
			};

			for (const auto& dir : directions)
			{
				sf::Vector2i targetSquare = sf::Vector2i(selectedPiece.x + dir[0], selectedPiece.y + dir[1]);

				if (targetSquare.x >= 1 && targetSquare.x <= 8 && targetSquare.y >= 1 && targetSquare.y <= 8)
				{
					Piece targetPiece = board[(int)targetSquare.y - 1][(int)targetSquare.x - 1];

					if (targetPiece.type != PieceType::None && targetPiece.color != board[(int)selectedPiece.y - 1][(int)selectedPiece.x - 1].color)
					{
						attackedSquares.push_back(targetSquare);
					}
				}
			}
		}
			break;
		default:
			break;
		}
	}

	return attackedSquares;
}

void Board::setPiece(int x, int y, Piece piece) {
    board[y][x] = piece;

	return;
}