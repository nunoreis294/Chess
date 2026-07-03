# Assistant Context

This file provides quick facts and codebase context that the automated assistant (GitHub Copilot) can read to help with requests.

- Project: Chess (C++ / SFML)
- Workspace root: repository root contains Visual Studio project Chess/Chess.vcxproj
- Build: Visual Studio (MSVC). SFML required.

Key files and responsibilities
- Chess/board.cpp/.h: board state, move generation, check/checkmate detection, moveHistory (notation), en passant & castling logic.
- Chess/game.cpp/.h: game flow and turn management. Use getBoard() to access board.
- Chess/gui.cpp/.h: rendering and user input (SFML). Uses selectedPiece as 1-based coords when interacting with UI.
- Chess/piece.h: PieceType and PieceColor enums, Piece struct.
- Chess/utils.h/.cpp: texture/resource loading.

Important conventions
- Internal board array indexing: board[y][x] with 0-based indices (0..7).
- UI coordinates (sf::Vector2i used across code) are 1-based when passed to getPossibleSquares/getAttackedSquares (values 1..8) and then converted internally by subtracting 1.
- moveHistory entries contain: notation, number, movedPiece, fromSquare (algebraic, e.g., "e2"), toSquare ("e4"), isCapture, isCheck, isCheckmate.

Helper functions to reuse
- Board::isKingChecked(PieceColor)
- Board::isKingCheckMated(PieceColor)
- Board::leavesKingInCheck(fromX,fromY,toX,toY) (0-based indices) and overload with sf::Vector2i (1-based -> converted)

Notes for changes
- Prefer adding minimal, localized changes. Use existing helper methods where available.
- When simulating captures (en passant or normal), ensure simulation removes the captured pawn where applicable before calling isKingChecked.
