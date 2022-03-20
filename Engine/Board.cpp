#include "Board.h"
#include <fstream>
#include <time.h>
#include "pieceValidator.h"

Board::Board(){
	resetBoard();
}
Board::~Board() {
	for (int i = 0; i < 64; i++) {
		if (board[i] != NULL) {
			delete board[i];
			board[i] = NULL;
		}
	}
}
Board& Board::operator=(const Board& b) {
	for (int i = 0; i < 64; i++) {
		delete board[i];
		if (b.board[i]->getPieceType() == "null") {
			board[i] = new NullPiece();
		}
		else if(b.board[i]->getPieceType() == "pawn") {
			board[i] = new Pawn(
			(b.board[i])->x,
			(b.board[i])->y,
			(b.board[i])->side,
			(b.board[i])->filename
		);
		}
		else if (b.board[i]->getPieceType() == "bishop") {
			board[i] = new Bishop(
				(b.board[i])->x,
				(b.board[i])->y,
				(b.board[i])->side,
				(b.board[i])->filename
			);
		}
		else if (b.board[i]->getPieceType() == "knight") {
			board[i] = new Knight(
				(b.board[i])->x,
				(b.board[i])->y,
				(b.board[i])->side,
				(b.board[i])->filename
			);
		}
		else if (b.board[i]->getPieceType() == "rook") {
			board[i] = new Rook(
				(b.board[i])->x,
				(b.board[i])->y,
				(b.board[i])->side,
				(b.board[i])->filename
			);
		}
		else if (b.board[i]->getPieceType() == "king") {
			board[i] = new King(
				(b.board[i])->x,
				(b.board[i])->y,
				(b.board[i])->side,
				(b.board[i])->filename
			);
		}
		else if (b.board[i]->getPieceType() == "queen") {
			board[i] = new Queen(
				(b.board[i])->x,
				(b.board[i])->y,
				(b.board[i])->side,
				(b.board[i])->filename
			);
		}
	}
	return *this;
}

//Move making
std::vector<int> Board::getLegalMoves(int x, int y) {
	board[x + y * 8]->calculateLegalMoves(board);
	std::vector<int> moves = board[x + y * 8]->legalMoves;
	std::vector<int> legalMoves;

	if ((board[x + y * 8]->side == 0 && whiteToMove) || board[x + y * 8]->side == 1 && !whiteToMove) {
		std::vector<int> noMoves;
		return noMoves;
	}
	else {
		for (size_t i = 0; i < moves.size(); i++) {
			int newPos = x + y * 8 + moves[i];
			int newX = newPos % 8;
			int newY = (int)(newPos / 8);

			if (moveIsLegal(x, y, newX, newY)) {
				legalMoves.push_back(moves[i]);
			}
		}
	}

	//Add castling
	if (board[x + y * 8]->getPieceType() == "king") {
		setCastlingOptions();

		if (board[x + y * 8]->side == 0) {
			if (blackCanCastleKingSide) legalMoves.push_back(2);
			if (blackCanCastleQueenSide) legalMoves.push_back(-2);
		}

		if (board[x + y * 8]->side == 1) {
			if (whiteCanCastleKingSide) legalMoves.push_back(2);
			if (whiteCanCastleQueenSide) legalMoves.push_back(-2);
		}
	}

	return legalMoves;
}
void Board::makeMove(int oldX, int oldY, int newX, int newY) {
	//add starting pozition
	if (game.size() == 0) {
		game.push_back("rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR");
	}

	if (board[oldX + oldY * 8]->getPieceType() == "null") {
		; //Nothing needs to be done
	}
	else
	if (moveIsLegal(oldX, oldY, newX, newY)) {

		//reset en-passant rights for all pawns
		for (int i = 0; i < 64; i++) {
			if (board[i]->getPieceType() == "pawn") {
				board[i]->canEnPassantLeft = false;
				board[i]->canEnPassantRight = false;
			}
		}

		 if (board[oldX + oldY * 8]->getPieceType() == "pawn") {
			//Copy
			Pawn pieceCopy = Pawn(
				newX,
				newY,
				(board[oldX + oldY * 8])->side,
				(board[oldX + oldY * 8])->filename
			);
			validator.validate(&pieceCopy);
			pieceCopy.canEnPassantLeft = (board[oldX + oldY * 8])->canEnPassantLeft;
			pieceCopy.canEnPassantRight = (board[oldX + oldY * 8])->canEnPassantRight;
			pieceCopy.hasMoved = (board[oldX + oldY * 8])->hasMoved;

			//En-passant
			if (((abs((oldX + oldY * 8) - (newX + newY * 8))) == 7 ||
				(abs((oldX + oldY * 8) - (newX + newY * 8))) == 9) &&
				board[newX + newY * 8]->getPieceType() == "null") {
				if (pieceCopy.side == 1) {
					delete board[newX + (newY + 1) * 8];
					board[newX + (newY + 1) * 8] = new NullPiece();
				}
				else {
					delete board[newX + (newY - 1) * 8];
					board[newX + (newY - 1) * 8] = new NullPiece();
				}
			}

			//Make the move
			delete board[newX + newY * 8];
			if ((board[oldX + oldY * 8]->side == 0 && newY == 7) ||
				(board[oldX + oldY * 8]->side == 1 && newY == 0)) {
				std::string queenFilename = (board[oldX + oldY * 8]->side == 0 ?
					"assets/blackQueen75x75.bmp" : "assets/whiteQueen75x75.bmp");
				//Delete
				delete board[oldX + oldY * 8];
				board[oldX + oldY * 8] = new NullPiece();

				board[newX + newY * 8] = new Queen(pieceCopy.x, pieceCopy.y, pieceCopy.side, queenFilename);
				validator.validate(board[newX + newY * 8]);
			}
			else {
				//Delete
				delete board[oldX + oldY * 8];
				board[oldX + oldY * 8] = new NullPiece();

				board[newX + newY * 8] = new Pawn(pieceCopy.x, pieceCopy.y, pieceCopy.side, pieceCopy.filename);
				validator.validate(board[newX + newY * 8]);
				board[newX + newY * 8]->hasMoved = true;
				board[newX + newY * 8]->canEnPassantLeft = pieceCopy.canEnPassantLeft;
				board[newX + newY * 8]->canEnPassantRight = pieceCopy.canEnPassantRight;
			}

			//Make the neighbooring pawns able to en-passant
			if (abs((oldX + oldY * 8) - (newX + newY * 8)) == 16) {
				if (whiteToMove) {
					if (board[newX + newY * 8 + 1]->getPieceType() == "pawn" &&
						board[newX + newY * 8 + 1]->side == 0) {
						board[newX + newY * 8 + 1]->canEnPassantLeft = true;
					}
					if (board[newX + newY * 8 - 1]->getPieceType() == "pawn" &&
						board[newX + newY * 8 - 1]->side == 0) {
						board[newX + newY * 8 - 1]->canEnPassantRight = true;
					}
				}
				else {
					if (board[newX + newY * 8 + 1]->getPieceType() == "pawn" &&
						board[newX + newY * 8 + 1]->side == 1) {
						board[newX + newY * 8 + 1]->canEnPassantLeft = true;
					}
					if (board[newX + newY * 8 - 1]->getPieceType() == "pawn" &&
						board[newX + newY * 8 - 1]->side == 1) {
						board[newX + newY * 8 - 1]->canEnPassantRight = true;
					}
				}
			}

			whiteToMove = (!whiteToMove);
		}
		else if (board[oldX + oldY * 8]->getPieceType() == "bishop") {
			//Copy
			Bishop pieceCopy = Bishop(
				newX,
				newY,
				(board[oldX + oldY * 8])->side,
				(board[oldX + oldY * 8])->filename
			);
			validator.validate(&pieceCopy);
			//Delete
			delete board[oldX + oldY * 8];
			board[oldX + oldY * 8] = new NullPiece();

			//Make the move
			delete board[newX + newY * 8];
			board[newX + newY * 8] = new Bishop(pieceCopy.x, pieceCopy.y, pieceCopy.side, pieceCopy.filename);
			validator.validate(board[newX + newY * 8]);
			whiteToMove = (!whiteToMove);
		}
		else if (board[oldX + oldY * 8]->getPieceType() == "knight") {
			//Copy
			Knight pieceCopy = Knight(
				newX,
				newY,
				(board[oldX + oldY * 8])->side,
				(board[oldX + oldY * 8])->filename
			);
			validator.validate(&pieceCopy);
			//Delete
			delete board[oldX + oldY * 8];
			board[oldX + oldY * 8] = new NullPiece();

			//Make the move
			delete board[newX + newY * 8];
			board[newX + newY * 8] = new Knight(pieceCopy.x, pieceCopy.y, pieceCopy.side, pieceCopy.filename);
			validator.validate(board[newX + newY * 8]);
			whiteToMove = (!whiteToMove);
		}
		else if (board[oldX + oldY * 8]->getPieceType() == "rook") {
			//Copy
			Rook pieceCopy = Rook(
				newX,
				newY,
				(board[oldX + oldY * 8])->side,
				(board[oldX + oldY * 8])->filename
			);
			validator.validate(&pieceCopy);
			//Delete
			delete board[oldX + oldY * 8];
			board[oldX + oldY * 8] = new NullPiece();

			//Make the move
			delete board[newX + newY * 8];
			board[newX + newY * 8] = new Rook(pieceCopy.x, pieceCopy.y, pieceCopy.side, pieceCopy.filename);
			validator.validate(board[newX + newY * 8]);
			whiteToMove = (!whiteToMove);
			board[newX + newY * 8]->hasMoved = true;
		}
		else if (board[oldX + oldY * 8]->getPieceType() == "king") {
			//Copy
			King pieceCopy = King(
				newX,
				newY,
				(board[oldX + oldY * 8])->side,
				(board[oldX + oldY * 8])->filename
			);
			validator.validate(&pieceCopy);
			//Delete
			delete board[oldX + oldY * 8];
			board[oldX + oldY * 8] = new NullPiece();

			//Make the move
			delete board[newX + newY * 8];
			board[newX + newY * 8] = new King(pieceCopy.x, pieceCopy.y, pieceCopy.side, pieceCopy.filename);
			validator.validate(board[newX + newY * 8]);
			//Other updates
			whiteToMove = (!whiteToMove);
			board[newX + newY * 8]->hasMoved = true;

			if (abs((newX + newY * 8) - (oldX + oldY * 8)) == 2) {
				if ((newX + newY * 8) == 62) {
					delete board[61];
					board[61] = new NullPiece();
					board[61] = new Rook(5, 7, board[63]->side, board[63]->filename);
					validator.validate(board[61]);
					board[61]->hasMoved = true;
					delete board[63];
					board[63] = new NullPiece();
				}

				if ((newX + newY * 8) == 6) {
					delete board[5];
					board[5] = new NullPiece();
					board[5] = new Rook(5, 0, board[7]->side, board[7]->filename);
					validator.validate(board[5]);
					board[5]->hasMoved = true;
					delete board[7];
					board[7] = new NullPiece();
				}

				if ((newX + newY * 8) == 58) {
					delete board[59];
					board[59] = new NullPiece();
					board[59] = new Rook(3, 7, board[56]->side, board[56]->filename);
					validator.validate(board[59]);
					board[59]->hasMoved = true;
					delete board[56];
					board[56] = new NullPiece();
				}

				if ((newX + newY * 8) == 2) {
					delete board[3];
					board[3] = new NullPiece();
					board[3] = new Rook(3, 0, board[0]->side, board[0]->filename);
					validator.validate(board[3]);
					board[3]->hasMoved = true;
					delete board[0];
					board[0] = new NullPiece();
				}
			}
		}
		else if (board[oldX + oldY * 8]->getPieceType() == "queen") {
			//Copy
			Queen pieceCopy = Queen(
				newX,
				newY,
				(board[oldX + oldY * 8])->side,
				(board[oldX + oldY * 8])->filename
			);
			validator.validate(&pieceCopy);
			//Delete
			delete board[oldX + oldY * 8];
			board[oldX + oldY * 8] = new NullPiece();

			//Make the move
			delete board[newX + newY * 8];
			board[newX + newY * 8] = new Queen(pieceCopy.x, pieceCopy.y, pieceCopy.side, pieceCopy.filename);
			validator.validate(board[newX + newY * 8]);
			whiteToMove = (!whiteToMove);
		}


		game.push_back(convertBoardToString());
	}
}
bool Board::kingCanBeCaptured(int kingColor, Piece* board[64]) {
	for (int i = 0; i < 64; i++) {
		if (!board[i]->isNull() && board[i]->side != kingColor) {
			board[i]->calculateLegalMoves(board);
			std::vector<int> moves = board[i]->legalMoves;
			for (size_t j = 0; j < moves.size(); j++) {
				if (board[i + moves[j]]->getPieceType() == "king" && board[i + moves[j]]->side == kingColor) {
					return true;
				}
			}
		}
	}
	return false;
}
bool Board::moveIsLegal(int oldX, int oldY, int newX, int newY) {
	if (board[oldX + oldY * 8]->side == 0 && whiteToMove) return false;
	if (board[oldX + oldY * 8]->side == 1 && !whiteToMove) return false;

	int pos1 = oldX + oldY * 8;
	int pos2 = newX + newY * 8;
	if(board[oldX + oldY*8]->getPieceType() == "pawn" &&
		board[newX + newY * 8]->getPieceType() == "null" &&
		(abs(pos1 - pos2) == 7 || abs(pos1 - pos2) == 9)) {
		return true;
	}

	bool legal = true;
	//Save piece data
	//Moving piece
	int x1 = board[oldX + oldY * 8]->x;
	int y1 = board[oldX + oldY * 8]->y;
	int side1 = board[oldX + oldY * 8]->side;
	std::string filename1 = board[oldX + oldY * 8]->filename;
	std::string type1 = board[oldX + oldY * 8]->getPieceType();
	bool hasMoved1 = board[oldX + oldY * 8]->hasMoved;
	bool canEnPassantLeft1 = board[oldX + oldY * 8]->canEnPassantLeft;
	bool canEnPassantRight1 = board[oldX + oldY * 8]->canEnPassantRight;
	//bool isInCheck1 = board[oldX + oldY * 8]->isInCheck;
	//Captured piece
	int x2 = board[newX + newY * 8]->x;
	int y2 = board[newX + newY * 8]->y;
	int side2 = board[newX + newY * 8]->side;
	std::string filename2 = board[newX + newY * 8]->filename;
	std::string type2 = board[newX + newY * 8]->getPieceType();
	bool hasMoved2 = board[newX + newY * 8]->hasMoved;
	bool canEnPassantLeft2 = board[newX + newY * 8]->canEnPassantLeft;
	bool canEnPassantRight2 = board[newX + newY * 8]->canEnPassantRight;
	//bool isInCheck2 = board[newX + newY * 8]->isInCheck;
	///////////////////

	//Make move
	delete board[oldX + oldY * 8];
	board[oldX + oldY * 8] = new NullPiece();

	delete board[newX + newY * 8];
	if (type1 == "pawn") {
		//Create new
		board[newX + newY * 8] = new Pawn(x1, y1, side1, filename1);
		//Check if king can be captured
		if (kingCanBeCaptured(side1, board)) {
			legal = false;
		}

		//Undo
		delete board[oldX + oldY * 8];
		board[oldX + oldY * 8] = new Pawn(x1, y1, side1, filename1);
		//Update the rest of the data
		board[oldX + oldY * 8]->hasMoved = hasMoved1;
		board[oldX + oldY * 8]->canEnPassantLeft = canEnPassantLeft1;
		board[oldX + oldY * 8]->canEnPassantRight = canEnPassantRight1;
		delete board[newX + newY * 8];
		if (type2 == "null") {
			board[newX + newY * 8] = new NullPiece();
		}
		else if (type2 == "pawn") {
			board[newX + newY * 8] = new Pawn(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			board[newX + newY * 8]->canEnPassantLeft = canEnPassantLeft2;
			board[newX + newY * 8]->canEnPassantRight = canEnPassantRight2;
		}
		else if (type2 == "rook"){
			board[newX + newY * 8] = new Rook(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
		}
		else if (type2 == "knight") {
			board[newX + newY * 8] = new Knight(x2, y2, side2, filename2);
		}
		else if (type2 == "bishop") {
			board[newX + newY * 8] = new Bishop(x2, y2, side2, filename2);
		}
		else if (type2 == "king") {
			board[newX + newY * 8] = new King(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			//board[newX + newY * 8]->isInCheck = isInCheck2;
		}
		else if (type2 == "queen") {
			board[newX + newY * 8] = new Queen(x2, y2, side2, filename2);
		}
	}
	else if (type1 == "rook") {
		board[newX + newY * 8] = new Rook(x1, y1, side1, filename1);
		//Check if king can be captured
		if (kingCanBeCaptured(side1, board)) {
			legal = false;
		}

		//Undo
		delete board[oldX + oldY * 8];
		board[oldX + oldY * 8] = new Rook(x1, y1, side1, filename1);
		board[oldX + oldY * 8]->hasMoved = hasMoved1;
		delete board[newX + newY * 8];
		if (type2 == "null") {
			board[newX + newY * 8] = new NullPiece();
		}
		else if (type2 == "pawn") {
			board[newX + newY * 8] = new Pawn(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			board[newX + newY * 8]->canEnPassantLeft = canEnPassantLeft2;
			board[newX + newY * 8]->canEnPassantRight = canEnPassantRight2;
		}
		else if (type2 == "rook") {
			board[newX + newY * 8] = new Rook(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
		}
		else if (type2 == "knight") {
			board[newX + newY * 8] = new Knight(x2, y2, side2, filename2);
		}
		else if (type2 == "bishop") {
			board[newX + newY * 8] = new Bishop(x2, y2, side2, filename2);
		}
		else if (type2 == "king") {
			board[newX + newY * 8] = new King(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			//board[newX + newY * 8]->isInCheck = isInCheck2;
		}
		else if (type2 == "queen") {
			board[newX + newY * 8] = new Queen(x2, y2, side2, filename2);
		}
	}
	else if (type1 == "knight") {
		board[newX + newY * 8] = new Knight(x1, y1, side1, filename1);
		//Check if king can be captured
		if (kingCanBeCaptured(side1, board)) {
			legal = false;
		}

		//Undo
		delete board[oldX + oldY * 8];
		board[oldX + oldY * 8] = new Knight(x1, y1, side1, filename1);
		delete board[newX + newY * 8];
		if (type2 == "null") {
			board[newX + newY * 8] = new NullPiece();
		}
		else if (type2 == "pawn") {
			board[newX + newY * 8] = new Pawn(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			board[newX + newY * 8]->canEnPassantLeft = canEnPassantLeft2;
			board[newX + newY * 8]->canEnPassantRight = canEnPassantRight2;
		}
		else if (type2 == "rook") {
			board[newX + newY * 8] = new Rook(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
		}
		else if (type2 == "knight") {
			board[newX + newY * 8] = new Knight(x2, y2, side2, filename2);
		}
		else if (type2 == "bishop") {
			board[newX + newY * 8] = new Bishop(x2, y2, side2, filename2);
		}
		else if (type2 == "king") {
			board[newX + newY * 8] = new King(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			//board[newX + newY * 8]->isInCheck = isInCheck2;
		}
		else if (type2 == "queen") {
			board[newX + newY * 8] = new Queen(x2, y2, side2, filename2);
		}
	}
	else if (type1 == "bishop") {
		board[newX + newY * 8] = new Bishop(x1, y1, side1, filename1);
		//Check if king can be captured
		if (kingCanBeCaptured(side1, board)) {
			legal = false;
		}

		//Undo
		delete board[oldX + oldY * 8];
		board[oldX + oldY * 8] = new Bishop(x1, y1, side1, filename1);
		delete board[newX + newY * 8];
		if (type2 == "null") {
			board[newX + newY * 8] = new NullPiece();
		}
		else if (type2 == "pawn") {
			board[newX + newY * 8] = new Pawn(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			board[newX + newY * 8]->canEnPassantLeft = canEnPassantLeft2;
			board[newX + newY * 8]->canEnPassantRight = canEnPassantRight2;
		}
		else if (type2 == "rook") {
			board[newX + newY * 8] = new Rook(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
		}
		else if (type2 == "knight") {
			board[newX + newY * 8] = new Knight(x2, y2, side2, filename2);
		}
		else if (type2 == "bishop") {
			board[newX + newY * 8] = new Bishop(x2, y2, side2, filename2);
		}
		else if (type2 == "king") {
			board[newX + newY * 8] = new King(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			//board[newX + newY * 8]->isInCheck = isInCheck2;
		}
		else if (type2 == "queen") {
			board[newX + newY * 8] = new Queen(x2, y2, side2, filename2);
		}
	}
	else if (type1 == "king") {
		board[newX + newY * 8] = new King(x1, y1, side1, filename1);
		//Check if king can be captured
		if (kingCanBeCaptured(side1, board)) {
			legal = false;
		}

		//Undo
		delete board[oldX + oldY * 8];
		board[oldX + oldY * 8] = new King(x1, y1, side1, filename1);
		board[oldX + oldY * 8]->hasMoved = hasMoved1;
		//board[oldX + oldY * 8]->isInCheck = isInCheck1;
		delete board[newX + newY * 8];
		if (type2 == "null") {
			board[newX + newY * 8] = new NullPiece();
		}
		else if (type2 == "pawn") {
			board[newX + newY * 8] = new Pawn(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			board[newX + newY * 8]->canEnPassantLeft = canEnPassantLeft2;
			board[newX + newY * 8]->canEnPassantRight = canEnPassantRight2;
		}
		else if (type2 == "rook") {
			board[newX + newY * 8] = new Rook(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
		}
		else if (type2 == "knight") {
			board[newX + newY * 8] = new Knight(x2, y2, side2, filename2);
		}
		else if (type2 == "bishop") {
			board[newX + newY * 8] = new Bishop(x2, y2, side2, filename2);
		}
		else if (type2 == "king") {
			board[newX + newY * 8] = new King(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			//board[newX + newY * 8]->isInCheck = isInCheck2;
		}
		else if (type2 == "queen") {
			board[newX + newY * 8] = new Queen(x2, y2, side2, filename2);
		}
	}
	else if (type1 == "queen") {
		board[newX + newY * 8] = new Queen(x1, y1, side1, filename1);
		//Check if king can be captured
		if (kingCanBeCaptured(side1, board)) {
			legal = false;
		}

		//Undo
		delete board[oldX + oldY * 8];
		board[oldX + oldY * 8] = new Queen(x1, y1, side1, filename1);
		delete board[newX + newY * 8];
		if (type2 == "null") {
			board[newX + newY * 8] = new NullPiece();
		}
		else if (type2 == "pawn") {
			board[newX + newY * 8] = new Pawn(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			board[newX + newY * 8]->canEnPassantLeft = canEnPassantLeft2;
			board[newX + newY * 8]->canEnPassantRight = canEnPassantRight2;
		}
		else if (type2 == "rook") {
			board[newX + newY * 8] = new Rook(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
		}
		else if (type2 == "knight") {
			board[newX + newY * 8] = new Knight(x2, y2, side2, filename2);
		}
		else if (type2 == "bishop") {
			board[newX + newY * 8] = new Bishop(x2, y2, side2, filename2);
		}
		else if (type2 == "king") {
			board[newX + newY * 8] = new King(x2, y2, side2, filename2);
			board[newX + newY * 8]->hasMoved = hasMoved2;
			//board[newX + newY * 8]->isInCheck = isInCheck2;
		}
		else if (type2 == "queen") {
			board[newX + newY * 8] = new Queen(x2, y2, side2, filename2);
		}
	}

	return legal;
}

//Castling
std::vector<int> Board::getSquaresAttackedBySide(int side) {
	std::vector<int> attackedSquares;

	for (int i = 0; i < 64; i++) {
		if (board[i]->side == side) {
			board[i]->calculateLegalMoves(board);
			std::vector<int> candidateMoves = board[i]->legalMoves;
			int currentPos = board[i]->x + board[i]->y * 8;
			for (size_t j = 0; j < candidateMoves.size(); j++) {
				int attackedSquare = currentPos + candidateMoves[j];
				attackedSquares.push_back(attackedSquare);
			}
		}
	}

	return attackedSquares;
}
void Board::setCastlingOptions() {
	//Setup
	bool wk = true;
	bool wq = true;
	bool bk = true;
	bool bq = true;

	std::vector<int> controledByBlack = getSquaresAttackedBySide(0);
	std::vector<int> controledByWhite = getSquaresAttackedBySide(1);

	//White: kingside
	for (size_t i = 0; i < controledByBlack.size(); i++) {
		if (controledByBlack[i] == 60 ||
			controledByBlack[i] == 61 ||
			controledByBlack[i] == 62) {
			wk = false;
		}
	}

	if (//60 is white king and it hasn't moved
		board[60]->getPieceType() != "king" ||
		board[60]->side != 1 ||
		board[60]->hasMoved ||
		//63 is white rook and it hasn't moved
		board[63]->getPieceType() != "rook" ||
		board[63]->side != 1 ||
		board[63]->hasMoved ||
		//no pieces between 60 and 63
		board[61]->getPieceType() != "null" ||
		board[62]->getPieceType() != "null") {
		wk = false;
	}

	whiteCanCastleKingSide = wk;

	//White: queenside
	for (size_t i = 0; i < controledByBlack.size(); i++) {
		if (controledByBlack[i] == 58 ||
			controledByBlack[i] == 59 ||
			controledByBlack[i] == 60) {
			wq = false;
		}
	}

	if (//60 is white king and it hasn't moved
		board[60]->getPieceType() != "king" ||
		board[60]->side != 1 ||
		board[60]->hasMoved ||
		//56 is white rook and it hasn't moved
		board[56]->getPieceType() != "rook" ||
		board[56]->side != 1 ||
		board[56]->hasMoved ||
		//there are no pieces between 60 and 56
		board[59]->getPieceType() != "null" ||
		board[58]->getPieceType() != "null" ||
		board[57]->getPieceType() != "null") {
		wq = false;
	}

	whiteCanCastleQueenSide = wq;

	//Black: kingside
	for (size_t i = 0; i < controledByWhite.size(); i++) {
		if (controledByWhite[i] == 4 ||
			controledByWhite[i] == 5 ||
			controledByWhite[i] == 6) {
			bk = false;
		}
	}

	if (//4 is black king and it hasn't moved
		board[4]->getPieceType() != "king" ||
		board[4]->side != 0 ||
		board[4]->hasMoved ||
		//7 is black rook and it hasn't moved
		board[7]->getPieceType() != "rook" ||
		board[7]->side != 0 ||
		board[7]->hasMoved ||
		//no pieces between 4 and 7
		board[5]->getPieceType() != "null" ||
		board[6]->getPieceType() != "null") {
		bk = false;
	}

	blackCanCastleKingSide = bk;

	//Black: queenside
	for (size_t i = 0; i < controledByWhite.size(); i++) {
		if (controledByWhite[i] == 2 ||
			controledByWhite[i] == 3 ||
			controledByWhite[i] == 4) {
			bq = false;
		}
	}

	if (//4 is black king and it hasn't moved
		board[4]->getPieceType() != "king" ||
		board[4]->side != 0 ||
		board[4]->hasMoved ||
		//0 is black rook and it hasn't moved
		board[0]->getPieceType() != "rook" ||
		board[0]->side != 0 ||
		board[0]->hasMoved ||
		//there are no pieces between 0 and 4
		board[1]->getPieceType() != "null" ||
		board[2]->getPieceType() != "null" ||
		board[3]->getPieceType() != "null") {
		bq = false;
	}

	blackCanCastleQueenSide = bq;
}

//Bot
void Board::makeRandomMove(int side) {
	if (sideHasLegalMoves(side)) {
		std::vector<int> move;
		srand(time(NULL));
		bool moveIsSelected = false;
		bool canMove = true;

		while (!moveIsSelected) {
			int pos = rand() % 64;
			if (board[pos]->side == side) {
				board[pos]->calculateLegalMoves(board);
				std::vector<int> possibleMoves = board[pos]->legalMoves;
				if (possibleMoves.size() > 0) {
					int candidateMoveIndex = rand() % possibleMoves.size();
					int newPos = pos + possibleMoves[candidateMoveIndex];

					if (moveIsLegal(board[pos]->x, board[pos]->y, newPos % 8, (int)(newPos / 8))) {
						moveIsSelected = true;
						move.push_back(board[pos]->x);
						move.push_back(board[pos]->y);
						move.push_back((newPos % 8));
						move.push_back((int)(newPos / 8));
					}
				}
			}
		}

		makeMove(move[0], move[1], move[2], move[3]);
	}
}

//Load game
void Board::resetBoard() {
	for (int i = 0; i < 64; i++) {
		if (board[i] != NULL) {
			delete board[i];
		}
	}
	board[0] = new Rook(0, 0, 0, "assets/blackRook75x75.bmp");
	board[1] = new Knight(1, 0, 0, "assets/blackKnight75x75.bmp");
	board[2] = new Bishop(2, 0, 0, "assets/blackBishop75x75.bmp");
	board[3] = new Queen(3, 0, 0, "assets/blackQueen75x75.bmp");
	board[4] = new King(4, 0, 0, "assets/blackKing75x75.bmp");
	board[5] = new Bishop(5, 0, 0, "assets/blackBishop75x75.bmp");
	board[6] = new Knight(6, 0, 0, "assets/blackKnight75x75.bmp");
	board[7] = new Rook(7, 0, 0, "assets/blackRook75x75.bmp");

	for (int i = 8; i < 16; i++) {
		board[i] = new Pawn(i % 8, 1, 0, "assets/blackPawn75x75.bmp");
	}

	for (int i = 16; i < 48; i++) {
		board[i] = new NullPiece();
	}

	for (int i = 48; i < 56; i++) {
		board[i] = new Pawn(i % 8, 6, 1, "assets/whitePawn75x75.bmp");
	}

	board[56] = new Rook(0, 7, 1, "assets/whiteRook75x75.bmp");
	board[57] = new Knight(1, 7, 1, "assets/whiteKnight75x75.bmp");
	board[58] = new Bishop(2, 7, 1, "assets/whiteBishop75x75.bmp");
	board[59] = new Queen(3, 7, 1, "assets/whiteQueen75x75.bmp");
	board[60] = new King(4, 7, 1, "assets/whiteKing75x75.bmp");
	board[61] = new Bishop(5, 7, 1, "assets/whiteBishop75x75.bmp");
	board[62] = new Knight(6, 7, 1, "assets/whiteKnight75x75.bmp");
	board[63] = new Rook(7, 7, 1, "assets/whiteRook75x75.bmp");
}
std::vector<std::string> Board::getGameFromFile(std::string filename) {
	std::vector<std::string> game;
	resetBoard();

	std::ifstream file(filename);

	if (file.is_open()) {
		std::string token, sep = " ";
		int pos = 0, skipCounter = 0;

		//Skip useless parts
		for (int i = 0; i < 12; i++) std::getline(file, token);
		file >> token;

		int side = 1;

		while (file >> token) {
			if ((skipCounter % 5) == 0) { // Skip the time stamp stuff
				//convert to usable format
				auto move = convertStandardNotationToMove(token);
				//play the move on the board
				makeMove(move.first.first, move.first.second, move.second.first, move.second.second);
				//add position to the game vector
				game.push_back(convertBoardToString());
			}

			skipCounter++;
		}
		file.close();
	}
	resetBoard();

	return game;
}
std::pair<std::pair<int, int>, std::pair<int, int>> Board::convertStandardNotationToMove(std::string move) {
	std::pair<std::pair<int, int>, std::pair<int, int>> result;
	//convert move
	std::string pieceType;
	int from = 0;
	int to = 0;

	if (move[0] >= 'a' && move[0] <= 'h') { //pawn move

		if (move[1] != 'x') { //Not a capture

			to += (move[0] - 'a');
			to += (7 - (move[1] - '1')) * 8;

			if (whiteToMove) {
				if (board[to + 8]->getPieceType() == "pawn" &&
					board[to + 8]->side == 1) {
					from = to + 8;
				}
				else if (board[to + 8]->getPieceType() == "null" &&
					board[to + 16]->getPieceType() == "pawn" &&
					board[to + 16]->side == 1) {
					from = to + 16;
				}
			}
			else {
				if (board[to - 8]->getPieceType() == "pawn" &&
					board[to - 8]->side == 0) {
					from = to - 8;
				}
				else if (board[to - 8]->getPieceType() == "null" &&
					board[to - 16]->getPieceType() == "pawn" &&
					board[to - 16]->side == 0) {
					from = to - 16;
				}
			}

		}
		else { //It doesn't matter if it's a regular capture or en-passant

			to += (move[2] - 'a');
			to += (7 - (move[3] - '1')) * 8;

			from += (move[0] - 'a');
			from += (7 - (move[3] - '1') + 1) * 8 - (whiteToMove ? 0 : 16);

		}

	} 
	else if (move[0] == 'O') { //Castling
		if (move.length() == 3 || move.length() == 4) { // O-O or O-O+ or O-O#
			from = 4 + (whiteToMove ? 7 : 0) * 8;
			to = 6 + (whiteToMove ? 7 : 0) * 8;
		}
		else if (move.length() == 5 || move.length() == 6) { // O-O-O or O-O-O+ or O-O-O#
			from = 4 + (whiteToMove ? 7 : 0) * 8;
			to = 2 + (whiteToMove ? 7 : 0) * 8;
		}
	}
	else { //Other piece moves

		if (move[0] == 'R') pieceType = "rook";
		else if (move[0] == 'N') pieceType = "knight";
		else if (move[0] == 'B') pieceType = "bishop";
		else if (move[0] == 'K') pieceType = "king";
		else if (move[0] == 'Q') pieceType = "queen";

		if (move[1] == 'x') { //Capture; only one piece that can do it
			
			to += (move[2] - 'a');
			to += (7 - (move[3] - '1')) * 8;

			for (int i = 0; i < 64; i++) {
				if (board[i]->getPieceType() == pieceType && ((board[i]->side == 1 && whiteToMove) || (board[i]->side == 0 && !whiteToMove))) {
					board[i]->calculateLegalMoves(board);
					auto possibleMoves = board[i]->legalMoves;

					for (size_t j = 0; j < possibleMoves.size(); j++) {
						if ((board[i]->x + board[i]->y * 8 + possibleMoves[j]) == to && moveIsLegal(board[i]->x, board[i]->y, to % 8, (int)(to / 8))) {
							from = to - possibleMoves[j];
						}
					}
				}
			}
		}
		else if (move[2] == 'x') {  //Capture; there are multiple piece that can make this move
			to += (move[3] - 'a');
			to += (7 - (move[4] - '1')) * 8;

			if (move[1] >= 'a' && move[1] <= 'h') {

				for (int i = 0; i < 8; i++) { //Check on the column
					if (board[i*8 + (move[1] - 'a')]->getPieceType() == pieceType && 
						(
						(board[i * 8 + (move[1] - 'a')]->side == 1 && whiteToMove) ||
						(board[i * 8 + (move[1] - 'a')]->side == 0 && !whiteToMove)
						)
						) {
						board[i*8 + (move[1] - 'a')]->calculateLegalMoves(board);
						auto possibleMoves = board[i * 8 + (move[1] - 'a')]->legalMoves;

						for (size_t j = 0; j < possibleMoves.size(); j++) {
							if ((board[i * 8 + (move[1] - 'a')]->x + board[i * 8 + (move[1] - 'a')]->y * 8 + possibleMoves[j]) == to &&
								moveIsLegal(board[i * 8 + (move[1] - 'a')]->x, board[i * 8 + (move[1] - 'a')]->y, to % 8, (int)(to / 8))) {
								from = to - possibleMoves[j];
							}
						}
					}
				}
			}
			else if (move[1] >= '1' && move[1] <= '8') {
				for (int i = 0; i < 8; i++) { //Check on the row
					if (board[i +(7 - (move[1] - '1'))*8]->getPieceType() == pieceType &&
						(
						(board[i + (7 - (move[1] - '1')) * 8]->side == 1 && whiteToMove) ||
							(board[i + (7 - (move[1] - '1')) * 8]->side == 0 && !whiteToMove)
							)
						) {
						board[i + (7 - (move[1] - '1')) * 8]->calculateLegalMoves(board);
						auto possibleMoves = board[i + (7 - (move[1] - '1')) * 8]->legalMoves;

						for (size_t j = 0; j < possibleMoves.size(); j++) {
							if ((board[i + (7 - (move[1] - '1')) * 8]->x + board[i + (7 - (move[1] - '1')) * 8]->y * 8 + possibleMoves[j]) == to &&
								moveIsLegal(board[i + (7 - (move[1] - '1')) * 8]->x, board[i + (7 - (move[1] - '1')) * 8]->y, to % 8, (int)(to / 8))) {
								from = to - possibleMoves[j];
							}
						}
					}
				}
			}
		}
		else if (move.length() == 4 && move[3] >= '1' && move[3] <= '9') { //Not a capture; multiple pieces can go there: N2f3, Qdc4
			to += (move[2] - 'a');
			to += (7 - (move[3] - '1')) * 8;

			if (move[1] >= 'a' && move[1] <= 'h') {

				for (int i = 0; i < 8; i++) { //Check on the column
					if (board[i * 8 + (move[1] - 'a')]->getPieceType() == pieceType &&
						(
						(board[i * 8 + (move[1] - 'a')]->side == 1 && whiteToMove) ||
							(board[i * 8 + (move[1] - 'a')]->side == 0 && !whiteToMove)
							)
						) {
						board[i * 8 + (move[1] - 'a')]->calculateLegalMoves(board);
						auto possibleMoves = board[i * 8 + (move[1] - 'a')]->legalMoves;

						for (size_t j = 0; j < possibleMoves.size(); j++) {
							if ((board[i * 8 + (move[1] - 'a')]->x + board[i * 8 + (move[1] - 'a')]->y * 8 + possibleMoves[j]) == to &&
								moveIsLegal(board[i * 8 + (move[1] - 'a')]->x, board[i * 8 + (move[1] - 'a')]->y, to % 8, (int)(to / 8))) {
								from = to - possibleMoves[j];
							}
						}
					}
				}
			}
			else if (move[1] >= '1' && move[1] <= '8') {
				for (int i = 0; i < 8; i++) { //Check on the row
					if (board[i + (7 - (move[1] - '1')) * 8]->getPieceType() == pieceType &&
						(
						(board[i + (7 - (move[1] - '1')) * 8]->side == 1 && whiteToMove) ||
							(board[i + (7 - (move[1] - '1')) * 8]->side == 0 && !whiteToMove)
							)
						) {
						board[i + (7 - (move[1] - '1')) * 8]->calculateLegalMoves(board);
						auto possibleMoves = board[i + (7 - (move[1] - '1')) * 8]->legalMoves;

						for (size_t j = 0; j < possibleMoves.size(); j++) {
							if ((board[i + (7 - (move[1] - '1')) * 8]->x + board[i + (7 - (move[1] - '1')) * 8]->y * 8 + possibleMoves[j]) == to &&
								moveIsLegal(board[i + (7 - (move[1] - '1')) * 8]->x, board[i + (7 - (move[1] - '1')) * 8]->y, to % 8, (int)(to / 8))) {
								from = to - possibleMoves[j];
							}
						}
					}
				}
			}
		}
		else { //Regular move: Bc4
			to += (move[1] - 'a');
			to += (7 - (move[2] - '1')) * 8;

			for (int i = 0; i < 64; i++) {
				if (board[i]->getPieceType() == pieceType && ((board[i]->side == 1 && whiteToMove) || (board[i]->side == 0 && !whiteToMove))) {
					board[i]->calculateLegalMoves(board);
					auto possibleMoves = board[i]->legalMoves;

					for (size_t j = 0; j < possibleMoves.size(); j++) {
						if ((board[i]->x + board[i]->y * 8 + possibleMoves[j]) == to && moveIsLegal(board[i]->x, board[i]->y, to % 8, (int)(to / 8))) {
							from = to - possibleMoves[j];
						}
					}
				}
			}
		}
	}

	result.first.first = from % 8;
	result.first.second = (int)(from / 8);
	result.second.first = to % 8;
	result.second.second = (int)(to / 8);

	return result;
}
std::string Board::convertBoardToString() {
	std::string position = "";

	for (int i = 0; i < 64; i++) {
		if (board[i]->side == 0) {
			if (board[i]->getPieceType() == "pawn") position += "p";
			else if (board[i]->getPieceType() == "rook") position += "r";
			else if (board[i]->getPieceType() == "bishop") position += "b";
			else if (board[i]->getPieceType() == "knight") position += "n";
			else if (board[i]->getPieceType() == "king") position += "k";
			else if (board[i]->getPieceType() == "queen") position += "q";
		}
		else if (board[i]->side == 1) {
			if (board[i]->getPieceType() == "pawn") position += "P";
			else if (board[i]->getPieceType() == "rook") position += "R";
			else if (board[i]->getPieceType() == "bishop") position += "B";
			else if (board[i]->getPieceType() == "knight") position += "N";
			else if (board[i]->getPieceType() == "king") position += "K";
			else if (board[i]->getPieceType() == "queen") position += "Q";
		}
		else if (board[i]->side == -1) {
			position += "0";
		}
	}

	return position;
}
void Board::loadBoardFromString(std::string position) {
	for (int i = 0; i < 64; i++) {
		delete board[i];

		if (position[i] == '0') board[i] = new NullPiece();
		else if (position[i] == 'r') board[i] = new Rook(i % 8, (int)(i / 8), 0, "assets/blackRook75x75.bmp");
		else if (position[i] == 'n') board[i] = new Knight(i % 8, (int)(i / 8), 0, "assets/blackKnight75x75.bmp");
		else if (position[i] == 'b') board[i] = new Bishop(i % 8, (int)(i / 8), 0, "assets/blackBishop75x75.bmp");
		else if (position[i] == 'k') board[i] = new King(i % 8, (int)(i / 8), 0, "assets/blackKing75x75.bmp");
		else if (position[i] == 'q') board[i] = new Queen(i % 8, (int)(i / 8), 0, "assets/blackQueen75x75.bmp");
		else if (position[i] == 'p') board[i] = new Pawn(i % 8, (int)(i / 8), 0, "assets/blackPawn75x75.bmp");
		else if (position[i] == 'R') board[i] = new Rook(i % 8, (int)(i / 8), 1, "assets/whiteRook75x75.bmp");
		else if (position[i] == 'N') board[i] = new Knight(i % 8, (int)(i / 8), 1, "assets/whiteKnight75x75.bmp");
		else if (position[i] == 'B') board[i] = new Bishop(i % 8, (int)(i / 8), 1, "assets/whiteBishop75x75.bmp");
		else if (position[i] == 'K') board[i] = new King(i % 8, (int)(i / 8), 1, "assets/whiteKing75x75.bmp");
		else if (position[i] == 'Q') board[i] = new Queen(i % 8, (int)(i / 8), 1, "assets/whiteQueen75x75.bmp");
		else if (position[i] == 'P') board[i] = new Pawn(i % 8, (int)(i / 8), 1, "assets/whitePawn75x75.bmp");
	}


}
void Board::getPrevPosition() {
	if (currentGameMove > 0) {
		loadBoardFromString(game[--currentGameMove]);
	}
}
void Board::getNextPosition() {
	if (currentGameMove < ((int)(game.size() - 1))) {
		loadBoardFromString(game[++currentGameMove]);
	}
}

//Decide game result
bool Board::sideHasLegalMoves(int side) {
	bool hasMoves = false;

	for (int i = 0; i < 64; i++) {
		if (board[i]->side == side) {
			board[i]->calculateLegalMoves(board);
			std::vector<int> candidateMoves = board[i]->legalMoves;
			for (size_t j = 0; j < candidateMoves.size(); j++) {
				int newPos = board[i]->x + board[i]->y * 8 + candidateMoves[j];
				if (moveIsLegal(board[i]->x, board[i]->y, newPos % 8, (int)(newPos / 8))) {
					hasMoves = true;
				}
			}
		}
	}

	return hasMoves;
}
bool Board::drawByInsufficientMaterial() {
	//white pieces
	bool whiteHasLightSquaredBishop = false;
	bool whiteHasDarkSquaredBishop = false;
	bool whiteHasKnight = false;
	bool whiteHasPawnRookOrQueen = false;
	//black pieces
	bool blackHasLightSquaredBishop = false;
	bool blackHasDarkSquaredBishop = false;
	bool blackHasKnight = false;
	bool blackHasPawnRookOrQueen = false;

	//Check which pieces are left on the board
	for (int i = 0; i < 64; i++) {
		//knights
		if (board[i]->getPieceType() == "knight") {
			if (board[i]->side == 0) {
				blackHasKnight = true;
			}
			else if (board[i]->side == 1) {
				whiteHasKnight = true;
			}
		}

		//bishops
		if (board[i]->getPieceType() == "bishop") {
			//light squared
			if (i % 2 == 1) {
				if (board[i]->side == 0) {
					blackHasDarkSquaredBishop = true;
				}
				else if (board[i]->side == 1) {
					whiteHasDarkSquaredBishop = true;
				}
			}
			//dark squared
			else {
				if (board[i]->side == 0) {
					blackHasLightSquaredBishop = true;
				}
				else if (board[i]->side == 1) {
					whiteHasLightSquaredBishop = true;
				}
			}
		}

		//pawns, rooks, queens
		if (board[i]->getPieceType() == "pawn" ||
			board[i]->getPieceType() == "rook" ||
			board[i]->getPieceType() == "queen") {
			if (board[i]->side == 0) {
				blackHasPawnRookOrQueen = true;
			}
			else if (board[i]->side == 1) {
				whiteHasPawnRookOrQueen = true;
			}
		}

	}

	//two bishops of opposite colors
	if (whiteHasDarkSquaredBishop && whiteHasLightSquaredBishop) return false;
	if (blackHasDarkSquaredBishop && blackHasLightSquaredBishop) return false;
	
	//bishop and knight
	if ((whiteHasDarkSquaredBishop || whiteHasLightSquaredBishop) &&
		whiteHasKnight) return false;
	if ((blackHasDarkSquaredBishop || blackHasLightSquaredBishop) &&
		blackHasKnight) return false;

	//other pieces
	if (whiteHasPawnRookOrQueen || blackHasPawnRookOrQueen) return false;

	//draw
	return true;
}
bool Board::drawByRepetition() {
	int counter = 0;

	for (int i = 0; i < (int)(game.size() - 1); i++) {
		if (game[i] == game[game.size() - 1]) {
			counter++;
		}
	}

	if (counter > 1) return true;
	
	return false;
}
int Board::getMatchResult() {
	
	if (!whiteToMove) {
		if (!sideHasLegalMoves(0)) {
			if (kingCanBeCaptured(0, board)) return 1; //White won
			return 0; //Stalemate
		}
	}
	
	//Draw
	if (drawByInsufficientMaterial() || drawByRepetition()) return 0;
	
	if (whiteToMove) {
		if (!sideHasLegalMoves(1)) {
			if (kingCanBeCaptured(1, board)) return -1; //Black won
			return 0; //Stalemate
		}
	}

	return 2; //Game can continue
}
