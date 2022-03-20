#include "Piece.h"

Piece::Piece() {
	x = -1;
	y = -1;
	side = -1;
}

Piece::Piece(int x, int y, int side, std::string filename) {
	this->x = x;
	this->y = y;
	this->side = side;
	this->filename = filename;
	this->img = Surface(this->filename);
}

Piece::Piece(const Piece& p) {
	x = p.x;
	y = p.y;
	side = p.side;
}

Piece& Piece::operator=(const Piece& p) {
	x = p.x;
	y = p.y;
	side = p.side;
	filename = p.filename;
	img = p.img;
	legalMoves = p.legalMoves;
	canEnPassantLeft = p.canEnPassantLeft;
	canEnPassantRight = p.canEnPassantRight;
	hasMoved = p.hasMoved;

	return *this;
}