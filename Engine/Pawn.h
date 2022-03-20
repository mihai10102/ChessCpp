#pragma once

#include "Piece.h"
#include "Surface.h"
#include <vector>

class Pawn : public Piece {
private:
	static constexpr int moves[] = {
		//For black
		8, 16,
		7, 9, // Captures/En-passant
		//For white
		-8,-16,
		-9, -7 // Captures/En-passant
	};
public:
	Pawn(int x, int y, int side, std::string filename)
		:
		Piece(x, y, side, filename)
	{}

	void calculateLegalMoves(Piece* board[64]) override;
	std::string getPieceType() override {
		return "pawn";
	}
};
