#pragma once

#include "Piece.h"
#include "Surface.h"
#include <vector>

class Rook : public Piece {
public:
	static constexpr int moves[] = {
		1, 2, 3, 4, 5, 6, 7,  //Moving to the right
		-1, -2, -3, -4, -5, -6, -7,  //Moving to the left
		8, 16, 24, 32, 40, 48, 56,  //Moving down
		-8, -16, -24, -32, -40, -48, -56  //Moving up
	};
public:
	Rook(int x, int y, int side, std::string filename) 
		:
	Piece(x, y, side, filename)
	{}

	void calculateLegalMoves(Piece* board[64]) override;
	std::string getPieceType() override {
		return "rook";
	}
};