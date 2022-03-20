#pragma once

#include "Piece.h"
#include "Surface.h"
#include <vector>

class Bishop : public Piece {
public:
	static constexpr int moves[] = {
		9, 18, 27, 36, 45, 54, 63, //Bottom-right diagonal
	   -9,-18,-27,-36,-45,-54,-63, //Top-left diagonal
	    7, 14, 21, 28, 35, 42, 49, //Bottom-left diagonal
	   -7,-14,-21,-28,-35,-42,-49, //Top-right diagonal
	};
public:
	Bishop(int x, int y, int side, std::string filename)
		:
		Piece(x, y, side, filename)
	{}

	void calculateLegalMoves(Piece* board[64]) override;
	std::string getPieceType() override {
		return "bishop";
	}
};
