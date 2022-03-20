#pragma once

#include "Piece.h"
#include "Surface.h"
#include <vector>

class King : public Piece {
public:
	static constexpr int moves[] = {
		1, -7, 9, //Right
		8, -8, //Up and Down
		- 1, -9, 7 //Left
	};
public:
	King(int x, int y, int side, std::string filename)
		:
		Piece(x, y, side, filename)
	{}

	void calculateLegalMoves(Piece* board[64]) override;
	std::string getPieceType() override {
		return "king";
	}
};
