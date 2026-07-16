#include "game.h"

#include <sstream>

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
	whiteTimeSeconds = 30;
	blackTimeSeconds = 30;
	history.clear();
	historyIndex = -1;
	lastClockUpdate = std::chrono::steady_clock::now();
	saveCurrentState();
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

	lastClockUpdate = std::chrono::steady_clock::now();
	return;
}

void Game::updateClock()
{
	if (gameResult.type != GameResultType::None)
		return;

	auto now = std::chrono::steady_clock::now();
	const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - lastClockUpdate).count();
	if (elapsedSeconds <= 0)
		return;

	if (currentPlayerColor == PlayerColor::White)
		whiteTimeSeconds = std::max(0, whiteTimeSeconds - static_cast<int>(elapsedSeconds));
	else if (currentPlayerColor == PlayerColor::Black)
		blackTimeSeconds = std::max(0, blackTimeSeconds - static_cast<int>(elapsedSeconds));

	lastClockUpdate = now;

	if (whiteTimeSeconds == 0)
	{
		gameResult = { GameResultType::Timeout, PieceColor::Black, "Black wins by timeout" };
	}
	else if (blackTimeSeconds == 0)
	{
		gameResult = { GameResultType::Timeout, PieceColor::White, "White wins by timeout" };
	}
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
	saveCurrentState();
}

bool Game::isGameOver() const
{
	return gameResult.type != GameResultType::None;
}

void Game::goToPreviousMove()
{
	if (canGoToPreviousMove())
	{
		--historyIndex;
		restoreStateAt(historyIndex);
	}
}

void Game::goToNextMove()
{
	if (canGoToNextMove())
	{
		++historyIndex;
		restoreStateAt(historyIndex);
	}
}

bool Game::canGoToPreviousMove() const
{
	return historyIndex > 0;
}

bool Game::canGoToNextMove() const
{
	return historyIndex >= 0 && historyIndex < (int)history.size() - 1;
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

int Game::getRemainingTimeSeconds(PlayerColor player) const
{
	if (player == PlayerColor::White)
		return whiteTimeSeconds;

	if (player == PlayerColor::Black)
		return blackTimeSeconds;

	return 0;
}

std::string Game::getFormattedTime(PlayerColor player) const
{
	const int remainingSeconds = getRemainingTimeSeconds(player);
	const int minutes = remainingSeconds / 60;
	const int seconds = remainingSeconds % 60;

	std::ostringstream stream;
	stream << (minutes < 10 ? "0" : "") << minutes << ":" << (seconds < 10 ? "0" : "") << seconds;
	return stream.str();
}

void Game::saveCurrentState()
{
	if (historyIndex >= 0 && historyIndex < (int)history.size() - 1)
		history.erase(history.begin() + historyIndex + 1, history.end());

	history.push_back({ board, currentPlayerColor, gameResult });
	historyIndex = (int)history.size() - 1;
}

void Game::restoreStateAt(int index)
{
	if (index < 0 || index >= (int)history.size())
		return;

	const GameSnapshot& snapshot = history[index];
	board = snapshot.board;
	currentPlayerColor = snapshot.currentPlayerColor;
	gameResult = snapshot.gameResult;
	historyIndex = index;
}
