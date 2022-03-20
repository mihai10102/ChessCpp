#include "King.h"

void King::calculateLegalMoves(Piece* board[64]) {
	legalMoves.clear();
	if (x != 7 && x != 0) {
		for (int i = 0; i < 8; i++) {
			if ((x + y * 8 + moves[i]) >= 0 && (x + y * 8 + moves[i]) < 64) {
				if (board[x + y * 8 + moves[i]]->side != side) {
					legalMoves.push_back(moves[i]);
				}
			}
		}
	}
	else if (x == 0) {
		for (int i = 0; i < 5; i++) {
			if ((x + y * 8 + moves[i]) >= 0 && (x + y * 8 + moves[i]) < 64) {
				if (board[x + y * 8 + moves[i]]->side != side) {
					legalMoves.push_back(moves[i]);
				}
			}
		}
	}
	else if (x == 7) {
		for (int i = 3; i < 8; i++) {
			if ((x + y * 8 + moves[i]) >= 0 && (x + y * 8 + moves[i]) < 64) {
				if (board[x + y * 8 + moves[i]]->side != side) {
					legalMoves.push_back(moves[i]);
				}
			}
		}
	}
}