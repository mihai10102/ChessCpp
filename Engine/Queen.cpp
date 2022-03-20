#include "Queen.h"

void Queen::calculateLegalMoves(Piece* board[64]) {
	legalMoves.clear();
	//Bottom-right diagonal
	bool canContinue = true;

	for (int i = 0; i < 7 && canContinue; i++) {
		if ((x + i >= 7) || (y + i >= 7)) {
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

	//Top-left diagonal
	canContinue = true;

	for (int i = 7; i < 14 && canContinue; i++) {
		if ((x - i <= 0 - 7) || (y - i <= 0 - 7)) {
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

	//Bottom-left diagonal
	canContinue = true;

	for (int i = 14; i < 21 && canContinue; i++) {
		if ((x - i <= 0 - 14) || (y + i >= 7 + 14)) {
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

	//Top-right diagonal
	canContinue = true;

	for (int i = 21; i < 28 && canContinue; i++) {
		if ((x + i >= 7 + 21) || (y - i <= 0 - 21)) {
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

	//Moving to the right
	canContinue = true;

	for (int i = 28; (i < 35) && canContinue; i++) {
		if (x + i >= 7 + 28) {
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

	for (int i = 35; (i < 42) && canContinue; i++) {
		if (x - i <= 0 - 35) {
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

	for (int i = 42; (i < 49) && canContinue; i++) {
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

	for (int i = 49; (i < 56) && canContinue; i++) {
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