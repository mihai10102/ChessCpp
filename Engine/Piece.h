#pragma once
#include <vector>
#include "Surface.h"

class Piece {
public:
	int x, y;
	int side;
	std::string filename;
	Surface img;
	std::vector<int> legalMoves;
	bool canEnPassantLeft = false;
	bool canEnPassantRight = false;
	bool hasMoved = false;
	//bool isInCheck = false;
public:
	Piece();
	Piece(int x, int y, int side, std::string filename);
	Piece(const Piece& p);
	Piece& operator= (const Piece& p);
	
	virtual void calculateLegalMoves(Piece* board[64]) = 0;
	bool isNull() { return (x == -1 && y == -1 && side == -1); }
	virtual std::string getPieceType() {
		return "piece";
	}
};