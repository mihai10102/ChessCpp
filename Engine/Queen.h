#pragma once

#include "Piece.h"
#include "Surface.h"
#include <vector>

class Queen : public Piece {
public:
	static constexpr int moves[] = {
		//Diagonal moves
		9, 18, 27, 36, 45, 54, 63, //Bottom-right diagonal
	   -9,-18,-27,-36,-45,-54,-63, //Top-left diagonal
		7, 14, 21, 28, 35, 42, 49, //Bottom-left diagonal
	   -7,-14,-21,-28,-35,-42,-49, //Bottom-right diagonal
		//Straight moves
		1, 2, 3, 4, 5, 6, 7,  //Moving to the right
		-1, -2, -3, -4, -5, -6, -7,  //Moving to the left
		8, 16, 24, 32, 40, 48, 56,  //Moving down
		-8, -16, -24, -32, -40, -48, -56  //Moving up
	};
public:
	Queen(int x, int y, int side, std::string filename)
		:
		Piece(x, y, side, filename)
	{}

	void calculateLegalMoves(Piece* board[64]) override;
	std::string getPieceType() override {
		return "queen";
	}
};
