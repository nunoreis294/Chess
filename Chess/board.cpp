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

	moveHistory.clear();
}

// Move a piece from (fromX, fromY) to (toX, toY)
bool Board::move(int fromX, int fromY, int toX, int toY)
{
	int number = (int)moveHistory.size() + 1;
	bool isCheck = false;
	bool isCheckmate = false;
	bool isCapture = board[toY][toX].type != PieceType::None;
	std::string notation = "";

	Piece movedPiece = board[fromY][fromX];

	std::string fromSquare = getSquareNotation(fromX, fromY);
	std::string toSquare = getSquareNotation(toX, toY);

	PieceType pieceType = board[fromY][fromX].type;
	PieceColor pieceColor = board[fromY][fromX].color;

	board[toY][toX] = { pieceType, pieceColor };
	board[fromY][fromX] = { PieceType::None, PieceColor::None };

	if (pieceColor == PieceColor::White)
		notation = std::to_string(number) + ". ";

	if (pieceType != PieceType::Pawn)
		notation += pieceType == PieceType::Knight ? 'N' :
		pieceType == PieceType::Bishop ? 'B' :
		pieceType == PieceType::Rook ? 'R' :
		pieceType == PieceType::Queen ? 'Q' : 'K';

	if (isCapture)
	{
		char captureChar = fromSquare[0];
		std::vector<sf::Vector2i> piecesOfSameType = getAttackingPieces(pieceColor, pieceType, toX, toY);

		if (piecesOfSameType.size() > 1)
		{
			for (sf::Vector2i pieceOfSameType : piecesOfSameType)
			{
				if (getSquareNotation(toX, toY)[0] == captureChar)
					captureChar = fromSquare[1];
			}

			notation += captureChar;
		}

		notation += 'x';
	}

	notation += toSquare;

	if (isKingCheckMated(pieceColor == PieceColor::White ? PieceColor::Black : PieceColor::White))
	{
		isCheckmate = true;
		notation += '#';
	}

	if (!isCheckmate && (isKingChecked(pieceColor == PieceColor::White ? PieceColor::Black : PieceColor::White)))
	{
		isCheck = true;
		notation += '+';
	}

	if (pieceColor == PieceColor::White)
		moveHistory.push_back({ notation, number, board[toY][toX], fromSquare, isCapture, toSquare, isCheck, isCheckmate });
	else
	{
		moveHistory.back().notation += " " + notation;
	}

	return true;
}

// Get the piece at position (x, y)
Piece Board::getPiece(int x, int y) const
{
	return board[y][x];
}

std::vector<Move> Board::getMoveHistory() const
{
	return moveHistory;
}

// Check if the king of the given color is in check
bool Board::isKingChecked(PieceColor pieceColor) const
{
	//TODO: Implement check detection logic
	return false;
}

// Check if the king of the given color is in checkmate
bool Board::isKingCheckMated(PieceColor pieceColor) const
{
	//TODO: Implement checkmate detection logic
	return false;
}

std::vector<sf::Vector2i> Board::getAttackingPieces(PieceColor pieceColor, PieceType pieceType, int toX, int toY) const
{
	//TODO: Implement logic to find all pieces of the given type and color that can attack the square (toX, toY) excluding the piece that is currently on that square


	return std::vector<sf::Vector2i>();
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
			if (isKingChecked(piece.color))
			{
				// King cannot move to a square that is under attack
				break;
			}

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

			//TODO: Implement castling logic
			//Castling consists of moving a king two squares towards the rook and executing a jump of a rook over the king.
			//the king and the relevant rook must not be moved, considered as castling rights inside a chess position
			//the king must not be in check nor may the king pass through a square that is attacked by an enemy piece
			//no square between king's start and final square may be controlled by the enemy
			if (piece.color == PieceColor::White && selectedPiece == sf::Vector2i(5, 1))
			{
				// Check for white castling
				if (board[0][4].type == PieceType::Rook && board[0][4].color == PieceColor::White &&
					board[0][3].type == PieceType::None && board[0][2].type == PieceType::None &&
					!isKingChecked(PieceColor::White))
				{
					possibleSquares.push_back(sf::Vector2i(3, 1)); // Queenside castling
				}
				if (board[0][7].type == PieceType::Rook && board[0][7].color == PieceColor::White &&
					board[0][6].type == PieceType::None &&
					!isKingChecked(PieceColor::White))
				{
					possibleSquares.push_back(sf::Vector2i(7, 1)); // Kingside castling
				}
			}
			else if (piece.color == PieceColor::Black && selectedPiece == sf::Vector2i(5, 8))
			{
				// Check for black castling
				if (board[7][4].type == PieceType::Rook && board[7][4].color == PieceColor::Black &&
					board[7][3].type == PieceType::None && board[7][2].type == PieceType::None &&
					!isKingChecked(PieceColor::Black))
				{
					possibleSquares.push_back(sf::Vector2i(3, 8)); // Queenside castling
				}
				if (board[7][7].type == PieceType::Rook && board[7][7].color == PieceColor::Black &&
					board[7][6].type == PieceType::None &&
					!isKingChecked(PieceColor::Black))
				{
					possibleSquares.push_back(sf::Vector2i(7, 8)); // Kingside castling
				}
			}
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

			//TODO: Implement en passant attack detection logic
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
			if (isKingChecked(piece.color))
			{
				// King cannot move to a square that is under attack
				break;
			}

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

std::string Board::getSquareNotation(int x, int y) const
{
	std::string notation;
	notation += (char)('a' + (x));
	notation += (char)('8' - (y));
	return notation;
}