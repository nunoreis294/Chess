#pragma once
#include <SFML/Graphics.hpp>
#include "piece.h"


struct Move {
    std::string notation;
    int number;
    Piece movedPiece;
    std::string fromSquare;
	bool isCapture;
    std::string toSquare;
	bool isCheck;
	bool isCheckmate;
};

class Board {
public:
    Board();
    void reset();
    bool move(int fromX, int fromY, int toX, int toY);
    Piece getPiece(int x, int y) const;
    std::vector<Move> getMoveHistory() const;
    bool isKingChecked(PieceColor pieceColor) const;
    bool isKingCheckMated(PieceColor pieceColor) const;
    bool isStalemated(PieceColor pieceColor) const;
    bool isInsufficientMaterial() const;
    bool leavesKingInCheck(int fromX, int fromY, int toX, int toY) const;
    bool leavesKingInCheck(const sf::Vector2i &from, const sf::Vector2i &to) const;
    std::vector<sf::Vector2i> getAttackingPieces(PieceColor pieceColor, PieceType pieceType, int toX, int toY) const;
    std::vector<sf::Vector2i> getPossibleSquares(sf::Vector2i selectedSquare) const;
    std::vector<sf::Vector2i> getAttackedSquares(sf::Vector2i selectedSquare) const;
    void setPiece(int x, int y, Piece piece);
    std::string getSquareNotation(int x, int y) const;

private:
    Piece board[8][8];
    std::vector<Move> moveHistory;
};
