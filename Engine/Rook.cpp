#include "Rook.h"

void Rook::calculateLegalMoves(Piece* board[64]) {
	legalMoves.clear();
	//Moving to the right
	bool canContinue = true;

	for (int i = 0; (i < 7) && canContinue; i++) {
		if (x + i >= 7) {
			canContinue = false;
		}
		else {
			if (board[x + y * 8 + moves[i]]->isNull()) {
				legalMoves.push_back(moves[i]);
			}
			else if (board[x + y * 8 + moves[i]]->side != side) {
				legalMoves.push_back(moves[i]);
				canContinue = false;
			}
			else {
				canContinue = false;
			}
		}
	}

	//Moving to the left
	canContinue = true;

	for (int i = 7; (i < 14) && canContinue; i++) {
		if (x - i <= 0 - 7) {
			canContinue = false;
		}
		else {
			if (board[x + y * 8 + moves[i]]->isNull()) {
				legalMoves.push_back(moves[i]);
			}
			else if (board[x + y * 8 + moves[i]]->side != side) {
				legalMoves.push_back(moves[i]);
				canContinue = false;
			}
			else {
				canContinue = false;
			}
		}
	}

	//Moving down
	canContinue = true;

	for (int i = 14; (i < 21) && canContinue; i++) {
		if (x + y * 8 + moves[i] >= 64) {
			canContinue = false;
		}
		else {
			if (board[x + y * 8 + moves[i]]->isNull()) {
				legalMoves.push_back(moves[i]);
			}
			else if (board[x + y * 8 + moves[i]]->side != side) {
				legalMoves.push_back(moves[i]);
				canContinue = false;
			}
			else {
				canContinue = false;
			}
		}
	}

	//Moving up
	canContinue = true;

	for (int i = 21; (i < 28) && canContinue; i++) {
		if (x + y * 8 + moves[i] < 0) {
			canContinue = false;
		}
		else {
			if (board[x + y * 8 + moves[i]]->isNull()) {
				legalMoves.push_back(moves[i]);
			}
			else if (board[x + y * 8 + moves[i]]->side != side) {
				legalMoves.push_back(moves[i]);
				canContinue = false;
			}
			else {
				canContinue = false;
			}
		}
	}
}