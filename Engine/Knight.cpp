#include "Knight.h"

void Knight::calculateLegalMoves(Piece* board[64]) {
	legalMoves.clear();
	int startI = 0, endI = 0;
	if (x == 7) {
		startI = 4;
		endI = 8;
	}
	else if (x == 6) {
		startI = 2;
		endI = 8;
	}
	else if (x >= 2) {
		startI = 0;
		endI = 8;
	}
	else if (x == 1) {
		startI = 0;
		endI = 6;
	}
	else if (x == 0) {
		startI = 0;
		endI = 4;
	}

	for (int i = startI; i < endI; i++) {
		int newPos = x + y * 8 + moves[i];
		if (newPos >= 0 && newPos < 64){
			if ((board[x + y * 8 + moves[i]]->isNull()) || board[x + y * 8 + moves[i]]->side != side) {
				legalMoves.push_back(moves[i]);
			}
		}
	}
}