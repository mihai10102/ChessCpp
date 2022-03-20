#include "Pawn.h"

void Pawn::calculateLegalMoves(Piece* board[64]) {
	legalMoves.clear();
	/* TODO: implement the calculation */

	//Moving forward
	if ((x + y * 8 + moves[0 + side * 4]) >= 0 && (x + y * 8 + moves[0 + side * 4]) < 64) {
		if (board[x + y * 8 + moves[0 + side * 4]]->isNull()) {
			legalMoves.push_back(moves[0 + side * 4]);
			if ((x + y * 8 + moves[1 + side * 4]) >= 0 && (x + y * 8 + moves[1 + side * 4]) < 64) {
				if (!hasMoved && board[x + y * 8 + moves[1 + side * 4]]->isNull()) {
					legalMoves.push_back(moves[1 + side * 4]);
				}
			}
		}
	}

	if ((x + y * 8 + moves[2 + side * 4]) >= 0 && (x + y * 8 + moves[2 + side * 4]) < 64) {
		if (x > 0) {
			//Capture
			if (!(board[x + y * 8 + moves[2 + side * 4]]->isNull()) && board[x + y * 8 + moves[2 + side * 4]]->side != side) {
				legalMoves.push_back(moves[2 + side * 4]);
			}
		}
	}

	if ((x + y * 8 + moves[3 + side * 4]) >= 0 && (x + y * 8 + moves[3 + side * 4]) < 64) {
		if (x < 7) {
			//Capture
			if (!(board[x + y * 8 + moves[3 + side * 4]]->isNull()) && board[x + y * 8 + moves[3 + side * 4]]->side != side) {
				legalMoves.push_back(moves[3 + side * 4]);
			}
		}
	}

	if (canEnPassantLeft) {
			legalMoves.push_back(moves[2 + side*4]);
	}

	if (canEnPassantRight) {
		legalMoves.push_back(moves[3 + side * 4]);
	}
}