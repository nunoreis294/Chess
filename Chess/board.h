#pragma once
#include <SFML/Graphics.hpp>
#include "piece.h"

class Board {
public:
    Board();
    void reset();
    bool move(int fromX, int fromY, int toX, int toY);
    Piece getPiece(int x, int y) const;
    bool isKingChecked(PieceColor pieceColor) const;
    std::vector<sf::Vector2f> getPossibleSquares(sf::Vector2f selectedSquare) const;
    std::vector<sf::Vector2f> getAttackedSquares(sf::Vector2f selectedSquare) const;

private:
    Piece board[8][8];
};
