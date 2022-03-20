#pragma once

#include "Piece.h"
#include "Surface.h"
#include <vector>

class NullPiece : public Piece {
public:
	NullPiece() : Piece(-1, -1, -1, "assets/whitePawn75x75.bmp"){}

	void calculateLegalMoves(Piece* board[64]) override{}
	std::string getPieceType() override {
		return "null";
	}
};
