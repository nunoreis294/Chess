#include "board.h"

// Constructor
Board::Board()
{
	reset();
}

// Simulate a move from (fromX,fromY) to (toX,toY) and return true if after the move
// the moving side's king is in check.
bool Board::leavesKingInCheck(int fromX, int fromY, int toX, int toY) const
{
	Board copy = *this;

	if (fromX < 0 || fromX >= 8 || fromY < 0 || fromY >= 8 || toX < 0 || toX >= 8 || toY < 0 || toY >= 8)
		return false;

	copy.board[toY][toX] = copy.board[fromY][fromX];
	copy.board[fromY][fromX] = { PieceType::None, PieceColor::None };

	Piece moved = copy.board[toY][toX];
	if (moved.type == PieceType::None) return false;

	return copy.isKingChecked(moved.color);
}

bool Board::leavesKingInCheck(const sf::Vector2i &from, const sf::Vector2i &to) const
{
	int fx = (int)from.x - 1;
	int fy = (int)from.y - 1;
	int tx = (int)to.x - 1;
	int ty = (int)to.y - 1;
	return leavesKingInCheck(fx, fy, tx, ty);
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

	if (isKingChecked(pieceColor))
	{
		board[toY][toX] = { PieceType::None, PieceColor::None };
		board[fromY][fromX] = { pieceType, pieceColor };

		return false;
	}		

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
	// Find king coordinates
	int kingX = -1, kingY = -1;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (board[y][x].type == PieceType::King && board[y][x].color == pieceColor)
			{
				kingX = x;
				kingY = y;
				break;
			}
		}
		if (kingX != -1) break;
	}

	if (kingX == -1) return false; // No king found, treat as not in check

	// Helper lambda to test whether square (tx,ty) is attacked by given color
	auto squareAttackedBy = [&](int tx, int ty, PieceColor attackerColor) {
		if (attackerColor == PieceColor::None) return false;

		// Pawn attacks
		int pawnDir = attackerColor == PieceColor::White ? -1 : 1;
		for (int dx : { -1, 1 })
		{
			int px = tx + dx;
			int py = ty - pawnDir; // pawn must be located one step behind the target in pawns' movement dir
			if (px >= 0 && px < 8 && py >= 0 && py < 8)
			{
				Piece p = board[py][px];
				if (p.type == PieceType::Pawn && p.color == attackerColor)
					return true;
			}
		}

		// Knight attacks
		int knightMoves[8][2] = { {1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2} };
		for (auto &m : knightMoves)
		{
			int nx = tx + m[0];
			int ny = ty + m[1];
			if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
			{
				Piece p = board[ny][nx];
				if (p.type == PieceType::Knight && p.color == attackerColor)
					return true;
			}
		}

		// Sliding pieces: bishop / rook / queen
		int bishopDirs[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
		for (auto &d : bishopDirs)
		{
			int x = tx + d[0];
			int y = ty + d[1];
			while (x >= 0 && x < 8 && y >= 0 && y < 8)
			{
				Piece p = board[y][x];
				if (p.type != PieceType::None)
				{
					if (p.color == attackerColor && (p.type == PieceType::Bishop || p.type == PieceType::Queen))
						return true;
					break;
				}
				x += d[0]; y += d[1];
			}
		}

		int rookDirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
		for (auto &d : rookDirs)
		{
			int x = tx + d[0];
			int y = ty + d[1];
			while (x >= 0 && x < 8 && y >= 0 && y < 8)
			{
				Piece p = board[y][x];
				if (p.type != PieceType::None)
				{
					if (p.color == attackerColor && (p.type == PieceType::Rook || p.type == PieceType::Queen))
						return true;
					break;
				}
				x += d[0]; y += d[1];
			}
		}

		// King attacks (adjacent)
		for (int dx = -1; dx <= 1; ++dx)
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (dx == 0 && dy == 0) continue;
			int kx = tx + dx;
			int ky = ty + dy;
			if (kx >= 0 && kx < 8 && ky >= 0 && ky < 8)
			{
				Piece p = board[ky][kx];
				if (p.type == PieceType::King && p.color == attackerColor)
					return true;
			}
		}

		return false;
	};

	PieceColor attacker = pieceColor == PieceColor::White ? PieceColor::Black : PieceColor::White;
	return squareAttackedBy(kingX, kingY, attacker);
}

// Check if the king of the given color is in checkmate
bool Board::isKingCheckMated(PieceColor pieceColor) const
{
	// If king is not in check, can't be checkmated
	if (!isKingChecked(pieceColor)) return false;

	// Helper to determine whether a single move from (fx,fy) to (tx,ty) is legal for the piece at fx,fy
	auto isLegalMove = [&](int fx, int fy, int tx, int ty) -> bool {
		if (fx == tx && fy == ty) return false;
		if (tx < 0 || tx >= 8 || ty < 0 || ty >= 8) return false;

		Piece p = board[fy][fx];
		if (p.type == PieceType::None || p.color != pieceColor) return false;

		Piece target = board[ty][tx];
		if (target.type != PieceType::None && target.color == pieceColor) return false; // cannot capture own piece

		int dx = tx - fx;
		int dy = ty - fy;

		switch (p.type)
		{
		case PieceType::Pawn: {
			int dir = p.color == PieceColor::White ? -1 : 1;
			// capture
			if (dy == dir && (dx == 1 || dx == -1) && target.type != PieceType::None && target.color != p.color)
				return true;
			// single forward
			if (dx == 0 && dy == dir && target.type == PieceType::None)
				return true;
			// double forward from initial rank
			int startRank = (p.color == PieceColor::White) ? 6 : 1;
			if (dx == 0 && dy == 2 * dir && fy == startRank && target.type == PieceType::None)
			{
				// ensure square in between is empty
				int midY = fy + dir;
				if (board[midY][fx].type == PieceType::None)
					return true;
			}
			return false;
		}
		case PieceType::Knight: {
			int adx = abs(dx), ady = abs(dy);
			return (adx == 1 && ady == 2) || (adx == 2 && ady == 1);
		}
		case PieceType::Bishop: {
			if (abs(dx) != abs(dy)) return false;
			int sx = (dx > 0) ? 1 : -1;
			int sy = (dy > 0) ? 1 : -1;
			int x = fx + sx, y = fy + sy;
			while (x != tx && y != ty)
			{
				if (board[y][x].type != PieceType::None) return false;
				x += sx; y += sy;
			}
			return true;
		}
		case PieceType::Rook: {
			if (dx != 0 && dy != 0) return false;
			int sx = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
			int sy = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
			int x = fx + sx, y = fy + sy;
			while (x != tx || y != ty)
			{
				if (board[y][x].type != PieceType::None) return false;
				x += sx; y += sy;
			}
			return true;
		}
		case PieceType::Queen: {
			if (dx == 0 || dy == 0 || abs(dx) == abs(dy))
			{
				int sx = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
				int sy = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
				int x = fx + sx, y = fy + sy;
				while (x != tx || y != ty)
				{
					if (board[y][x].type != PieceType::None) return false;
					x += sx; y += sy;
				}
				return true;
			}
			return false;
		}
		case PieceType::King: {
			return std::max(abs(dx), abs(dy)) == 1;
		}
		default:
			return false;
		}
	};

	// Try every legal move for every piece of this color; if any move results in king not being in check, it's not checkmate
	for (int fy = 0; fy < 8; ++fy)
	for (int fx = 0; fx < 8; ++fx)
	{
		Piece p = board[fy][fx];
		if (p.type == PieceType::None || p.color != pieceColor) continue;

		for (int ty = 0; ty < 8; ++ty)
		for (int tx = 0; tx < 8; ++tx)
		{
			if (!isLegalMove(fx, fy, tx, ty)) continue;

			// simulate move on a copy
			Board copy = *this;
			copy.board[ty][tx] = copy.board[fy][fx];
			copy.board[fy][fx] = { PieceType::None, PieceColor::None };

			// If after the move the king is not in check, not checkmate
			if (!copy.isKingChecked(pieceColor))
				return false;
		}
	}

	return true;
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
		int fx = (int)selectedPiece.x - 1;
		int fy = (int)selectedPiece.y - 1;

		switch (piece.type)
		{
		case PieceType::Pawn: {
			int direction = piece.color == PieceColor::White ? -1 : 1;
			sf::Vector2i forwardSquare = sf::Vector2i(selectedPiece.x, selectedPiece.y + direction);

				if (forwardSquare.y >= 1 && forwardSquare.y <= 8 && board[(int)forwardSquare.y - 1][(int)forwardSquare.x - 1].type == PieceType::None)
				{
					int tx = (int)forwardSquare.x - 1;
					int ty = (int)forwardSquare.y - 1;
					if (!leavesKingInCheck(fx, fy, tx, ty))
						possibleSquares.push_back(forwardSquare);

					// Check for initial double move
					if ((piece.color == PieceColor::White && selectedPiece.y == 7) ||
						(piece.color == PieceColor::Black && selectedPiece.y == 2))
					{
						sf::Vector2i doubleForwardSquare = sf::Vector2i(selectedPiece.x, selectedPiece.y + 2 * direction);

						if (board[(int)doubleForwardSquare.y - 1][(int)doubleForwardSquare.x - 1].type == PieceType::None)
						{
							int dtx = (int)doubleForwardSquare.x - 1;
							int dty = (int)doubleForwardSquare.y - 1;
							if (!leavesKingInCheck(fx, fy, dtx, dty))
								possibleSquares.push_back(doubleForwardSquare);
						}
					}
				}

			// Captures (including en passant)
			for (int dx : { -1, 1 })
			{
				sf::Vector2i diag = sf::Vector2i(selectedPiece.x + dx, selectedPiece.y + direction);
				if (diag.x >= 1 && diag.x <= 8 && diag.y >= 1 && diag.y <= 8)
				{
					Piece targetPiece = board[(int)diag.y - 1][(int)diag.x - 1];
					int tx = (int)diag.x - 1;
					int ty = (int)diag.y - 1;
					if (targetPiece.type != PieceType::None && targetPiece.color != piece.color)
					{
						if (!leavesKingInCheck(fx, fy, tx, ty))
							possibleSquares.push_back(diag);
					}
					else
					{
						// en passant: check adjacent pawn that moved two squares in the last move
						int adjX = (int)selectedPiece.x - 1 + dx;
						int adjY = (int)selectedPiece.y - 1; // same rank as pawn
						if (adjX >= 0 && adjX < 8 && adjY >= 0 && adjY < 8 && !moveHistory.empty())
						{
							Move last = moveHistory.back();
							if (last.movedPiece.type == PieceType::Pawn && last.movedPiece.color != piece.color)
							{
								// parse last.toSquare and last.fromSquare
								if (last.toSquare.size() >= 2 && last.fromSquare.size() >= 2)
								{
									int ltx = last.toSquare[0] - 'a';
									int lty = '8' - last.toSquare[1];
									int lfx = last.fromSquare[0] - 'a';
									int lfy = '8' - last.fromSquare[1];
									// check that the pawn moved to the adjacent square and it was a double move
									if (ltx == adjX && lty == adjY && abs(lfy - lty) == 2)
									{
										// landing square is diag (tx,ty)
										if (!leavesKingInCheck(fx, fy, tx, ty))
											possibleSquares.push_back(diag);
									}
								}
							}
						}
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
						int tx = (int)targetSquare.x - 1;
						int ty = (int)targetSquare.y - 1;
						if (!leavesKingInCheck(fx, fy, tx, ty))
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
						int tx = (int)targetSquare.x - 1;
						int ty = (int)targetSquare.y - 1;
						if (!leavesKingInCheck(fx, fy, tx, ty))
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
						int tx = (int)targetSquare.x - 1;
						int ty = (int)targetSquare.y - 1;
						if (!leavesKingInCheck(fx, fy, tx, ty))
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
						int tx = (int)targetSquare.x - 1;
						int ty = (int)targetSquare.y - 1;
						if (!leavesKingInCheck(fx, fy, tx, ty))
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
					{
						int tx = (int)targetSquare.x - 1;
						int ty = (int)targetSquare.y - 1;
						if (!leavesKingInCheck(fx, fy, tx, ty))
							possibleSquares.push_back(targetSquare);
					}
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
		int fx = (int)selectedPiece.x - 1;
		int fy = (int)selectedPiece.y - 1;

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
					int tx = (int)captureLeft.x - 1;
					int ty = (int)captureLeft.y - 1;
					if (!leavesKingInCheck(fx, fy, tx, ty))
						attackedSquares.push_back(captureLeft);
				}
			}

			if (captureRight.x >= 1 && captureRight.x <= 8 && captureRight.y >= 1 && captureRight.y <= 8)
			{
				Piece targetPiece = board[(int)captureRight.y - 1][(int)captureRight.x - 1];

				if (targetPiece.type != PieceType::None && targetPiece.color != board[(int)selectedPiece.y - 1][(int)selectedPiece.x - 1].color)
				{
					int tx = (int)captureRight.x - 1;
					int ty = (int)captureRight.y - 1;
					if (!leavesKingInCheck(fx, fy, tx, ty))
						attackedSquares.push_back(captureRight);
				}
			}

			// en passant: check adjacent pawn that moved two squares in the last move
			for (int dx : { -1, 1 })
			{
				int adjX = fx + dx;
				int adjY = fy; // adjacent pawn sits on same rank
				int landX = fx + dx;
				int landY = fy + direction; // landing diagonal square (0-based)
				if (adjX >= 0 && adjX < 8 && adjY >= 0 && adjY < 8 && landX >= 0 && landX < 8 && landY >= 0 && landY < 8 && !moveHistory.empty())
				{
					Move last = moveHistory.back();
					if (last.movedPiece.type == PieceType::Pawn && last.movedPiece.color != piece.color && last.toSquare.size() >= 2 && last.fromSquare.size() >= 2)
					{
						int ltx = last.toSquare[0] - 'a';
						int lty = '8' - last.toSquare[1];
						int lfx = last.fromSquare[0] - 'a';
						int lfy = '8' - last.fromSquare[1];
						// last moved pawn must have landed on the adjacent square and it must have been a double move
						if (ltx == adjX && lty == adjY && abs(lfy - lty) == 2)
						{
							// ensure landing diagonal square is inside board and that capture square is currently empty
							if (board[landY][landX].type == PieceType::None)
							{
								if (!leavesKingInCheck(fx, fy, landX, landY))
									attackedSquares.push_back(sf::Vector2i(landX + 1, landY + 1));
							}
						}
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
						{
							int tx = (int)targetSquare.x - 1;
							int ty = (int)targetSquare.y - 1;
							if (!leavesKingInCheck(fx, fy, tx, ty))
								attackedSquares.push_back(targetSquare);
						}

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
						{
							int tx = (int)targetSquare.x - 1;
							int ty = (int)targetSquare.y - 1;
							if (!leavesKingInCheck(fx, fy, tx, ty))
								attackedSquares.push_back(targetSquare);
						}

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
						{
							int tx = (int)targetSquare.x - 1;
							int ty = (int)targetSquare.y - 1;
							if (!leavesKingInCheck(fx, fy, tx, ty))
								attackedSquares.push_back(targetSquare);
						}

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
						{
							int tx = (int)targetSquare.x - 1;
							int ty = (int)targetSquare.y - 1;
							if (!leavesKingInCheck(fx, fy, tx, ty))
								attackedSquares.push_back(targetSquare);
						}
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