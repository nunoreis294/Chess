#pragma once
#include "piece.h"

class Board {
public:
    Board();
    void reset();
    bool move(int fromX, int fromY, int toX, int toY);
    Piece getPiece(int x, int y) const;
    // Add more as needed

private:
    Piece board[8][8];
};
