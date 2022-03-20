#pragma once

#include "Piece.h"
#include "Surface.h"
#include <vector>

class Knight : public Piece {
public:
	static constexpr int moves[] = {
		 - 6, 10, //Shifts columns 2 to the right
		 -15, 17, //Shifts columns 1 to the right
		  15,-17, //Shifts columns 1 to the left
		   6,-10, //Shifts columns 2 to the left
	};
public:
	Knight(int x, int y, int side, std::string filename)
		:
	Piece(x, y, side, filename)
	{}

	void calculateLegalMoves(Piece* board[64]) override;
	std::string getPieceType() override {
		return "knight";
	}
};
