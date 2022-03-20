#include "Tests.h"
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"
#include <assert.h>
#include "MyException.h"
#include "pieceValidator.h"
#include "Board.h"

//Pieces
void testPawn() {
	pieceValidator validator;
	Pawn p(0, 6, 1, "assets/whitePawn75x75.bmp");

	assert(p.x == 0);
	assert(p.y == 6);
	assert(p.side == 1);
	assert(p.filename == "assets/whitePawn75x75.bmp");

	assert(p.getPieceType() == "pawn");

	try {
		Pawn p2(2, 7, 1, "");
		validator.validate(&p2);
		assert(false);
	}
	catch (MyException&) {
		assert(true);
	}
	catch (...) {
		assert(false);
	}

	Board b;
	p.calculateLegalMoves(b.board);
	auto moves = p.legalMoves;
	assert(moves.size() == 2);
	assert(moves[0] == -8);
	assert(moves[1] == -16);

	b.loadBoardFromString("000000000000k00p00R0n0p00BP0000000P0000000K00P000000000Pr0000000");
	p.x = 2;
	p.y = 4;
	p.calculateLegalMoves(b.board);
	moves.clear();
	moves = p.legalMoves;
	assert(moves.size() == 0);

}
void testRook() {
	pieceValidator validator;
	Rook r(0, 7, 1, "assets/whiteRook75x75.bmp");

	assert(r.x == 0);
	assert(r.y == 7);
	assert(r.side == 1);
	assert(r.filename == "assets/whiteRook75x75.bmp");

	assert(r.getPieceType() == "rook");

	try {
		Rook r2(-2, 7, 5, "");
		validator.validate(&r2);
		assert(false);
	}
	catch (MyException&) {
		assert(true);
	}
	catch (...) {
		assert(false);
	}

	Board b;
	r.calculateLegalMoves(b.board);
	auto moves = r.legalMoves;
	assert(moves.size() == 0);

	b.loadBoardFromString("000000000000k00p00R0n0p00BP0000000P0000000K00P000000000Pr0000000");
	r.x = 2;
	r.y = 2;
	r.calculateLegalMoves(b.board);
	moves.clear();
	moves = r.legalMoves;
	assert(moves.size() == 6);

}
void testBishop() {
	pieceValidator validator;
	Bishop b(2, 7, 1, "assets/whiteBishop75x75.bmp");

	assert(b.x == 2);
	assert(b.y == 7);
	assert(b.side == 1);
	assert(b.filename == "assets/whiteBishop75x75.bmp");

	assert(b.getPieceType() == "bishop");

	try {
		Bishop b2(-2, 7, 5, "");
		validator.validate(&b2);
		assert(false);
	}
	catch (MyException&) {
		assert(true);
	}
	catch (...) {
		assert(false);
	}

	Board bo;
	b.calculateLegalMoves(bo.board);
	auto moves = b.legalMoves;
	assert(moves.size() == 0);

	bo.loadBoardFromString("000000000000k00p00R0n0p00BP0000000P0000000K00P000000000Pr0000000");
	b.x = 1;
	b.y = 3;
	b.calculateLegalMoves(bo.board);
	moves.clear();
	moves = b.legalMoves;
	assert(moves.size() == 2);
	assert(moves[0] == -9);
	assert(moves[1] == 7);

}
void testKnight() {
	pieceValidator validator;
	Knight n(1, 7, 1, "assets/whiteKnight75x75.bmp");

	assert(n.x == 1);
	assert(n.y == 7);
	assert(n.side == 1);
	assert(n.filename == "assets/whiteKnight75x75.bmp");

	assert(n.getPieceType() == "knight");

	try {
		Pawn n2(-2, 7, 5, "");
		validator.validate(&n2);
		assert(false);
	}
	catch (MyException&) {
		assert(true);
	}
	catch (...) {
		assert(false);
	}

	Board b;
	n.calculateLegalMoves(b.board);
	auto moves = n.legalMoves;
	assert(moves.size() == 2);
	assert(moves[0] == -15);
	assert(moves[1] == -17);

	b.loadBoardFromString("000000000000k00p00R0n0p00BP0000000P0000000K00P000000000Pr0000000");
	n.x = 4;
	n.y = 2;
	n.side = 0;
	n.calculateLegalMoves(b.board);
	moves.clear();
	moves = n.legalMoves;
	assert(moves.size() == 8);

}
void testQueen() {
	pieceValidator validator;
	Queen q(3, 7, 1, "assets/whiteQueen75x75.bmp");

	assert(q.x == 3);
	assert(q.y == 7);
	assert(q.side == 1);
	assert(q.filename == "assets/whiteQueen75x75.bmp");

	assert(q.getPieceType() == "queen");

	try {
		Queen q2(-2, 7, 5, "");
		validator.validate(&q2);
		assert(false);
	}
	catch (MyException&) {
		assert(true);
	}
	catch (...) {
		assert(false);
	}

	Board b;
	q.calculateLegalMoves(b.board);
	auto moves = q.legalMoves;
	assert(moves.size() == 0);

	b.loadBoardFromString("Q00000000000k00p00R0n0p00BP0000000P0000000K00P000000000Pr0000000");
	q.x = 0;
	q.y = 0;
	q.calculateLegalMoves(b.board);
	moves.clear();
	moves = q.legalMoves;
	assert(moves.size() == 15);

}
void testKing() {
	pieceValidator validator;
	King k(4, 7, 1, "assets/whiteKing75x75.bmp");

	assert(k.x == 4);
	assert(k.y == 7);
	assert(k.side == 1);
	assert(k.filename == "assets/whiteKing75x75.bmp");

	assert(k.getPieceType() == "king");

	try {
		King k2(-2, 7, 5, "");
		validator.validate(&k2);
		assert(false);
	}
	catch (MyException&) {
		assert(true);
	}
	catch (...) {
		assert(false);
	}

	Board b;
	k.calculateLegalMoves(b.board);
	auto moves = k.legalMoves;
	assert(moves.size() == 0);

	b.loadBoardFromString("000000000000k00p00R0n0p00B00000000P0000000K00P000000000Pr0000000");
	k.x = 2;
	k.y = 5;
	k.calculateLegalMoves(b.board);
	moves.clear();
	moves = k.legalMoves;
	assert(moves.size() == 7);

}

//Board
void testBoard() {
	Board b;

	for (int i = 0; i < 64; i++) {
		if (i >= 16 && i < 48) {
			assert(b.board[i]->getPieceType() == "null");
		}
		else {
			assert(b.board[i]->getPieceType() != "null");
		}
	}

	//getLegalMoves
	auto m1 = b.getLegalMoves(1, 0);
	assert(m1.size() == 0); //Because it's white's turn
	auto m2 = b.getLegalMoves(1, 7);
	assert(m2.size() == 2);
	assert(m2[0] == -15);
	assert(m2[1] == -17);

	/* ^^^ 1 bug found ^^^ */

	//makeMove
	b.makeMove(1, 0, 0, 2); //Legal but it's the other side's turn
	assert(b.board[1]->getPieceType() == "knight");
	assert(b.board[16]->getPieceType() == "null");
	b.makeMove(4, 6, 4, 4); //Legal move: 1. e4
	assert(b.board[52]->getPieceType() == "null");
	assert(b.board[36]->getPieceType() == "pawn");

	/* ^^^ 3 bugs found ^^^ */

	//moveIsLegal
	assert(!b.moveIsLegal(4, 4, 4, 3)); //white piece, but black to move
	b.makeMove(4, 1, 4, 3); // 1... e5
	b.makeMove(1, 7, 2, 5); // 2. Nc3
	b.makeMove(3, 0, 7, 4); // 2... Qh4
	assert(!b.moveIsLegal(5, 6, 5, 5)); // The pawn is pinned by the queen
	assert(b.moveIsLegal(6, 6, 6, 5)); //Attacking the queen

	/* ^^^ 0 bugs found ^^^ */

	//loadBoardFromString + resetBoard
	b.resetBoard();
	for (int i = 0; i < 64; i++) {
		if (i >= 16 && i < 48) {
			assert(b.board[i]->getPieceType() == "null");
		}
		else {
			assert(b.board[i]->getPieceType() != "null");
		}
	}
	b.loadBoardFromString("rnb0kbnrpppp0ppp000000000000p0000000P00q00N00000PPPP00PPR0BQKBNR");
	assert(b.kingCanBeCaptured(1, b.board));
	b.loadBoardFromString("rnb0kbnrpppp0ppp000000000000p0000000P00q00N00000PPPP0PPPR0BQKBNR");
	assert(!b.kingCanBeCaptured(1, b.board));

	/* ^^^ 0 bugs found ^^^ */


	//makeRandomMove
	b.resetBoard();
	b.whiteToMove = true;
	b.makeRandomMove(1);
	assert(b.convertBoardToString() != "rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR");
	b.loadBoardFromString("rnbqk0nrpppp0Qp00000000p00b0p00000B0P00000000000PPPP0PPPRNB0K0NR");
	b.makeRandomMove(0);
	assert(b.convertBoardToString() == "rnbqk0nrpppp0Qp00000000p00b0p00000B0P00000000000PPPP0PPPRNB0K0NR"); // There are no legal moves for the bot to make
	//sideHasLegalMoves + getMatchResult
	b.whiteToMove = false;
	assert(!b.sideHasLegalMoves(0));
	assert(b.getMatchResult() == 1); //white won

	b.loadBoardFromString("0000000000000Q0000000000000000000000PK0k00000000PPPP0PPPRNB00BNR");
	assert(!b.sideHasLegalMoves(0));
	assert(b.getMatchResult() == 0); //stalemate

	/* ^^^ 1 bug found ^^^ */

	//convertStardartNotationToMove
	
	b.loadBoardFromString("r0bqkb0rpppppppp00000n00000P00000n0000P00000000PPPP0PP00RNBQKBNR");
	auto move = b.convertStandardNotationToMove("Nfxd5");
	assert(move.first.first == 5);
	assert(move.first.second == 2);
	assert(move.second.first == 3);
	assert(move.second.second == 3);
	move = b.convertStandardNotationToMove("Nbxd5");
	assert(move.first.first == 1);
	assert(move.first.second == 4);
	assert(move.second.first == 3);
	assert(move.second.second == 3);
	b.loadBoardFromString("r0bqkb0rpppppppp0n000000000000000n00P0P000000Q0PPPP00P00RNB0KBNR");
	move = b.convertStandardNotationToMove("Nxc2+");
	assert(move.first.first == 1);
	assert(move.first.second == 4);
	assert(move.second.first == 2);
	assert(move.second.second == 6);
	move = b.convertStandardNotationToMove("N6d5");
	assert(move.first.first == 1);
	assert(move.first.second == 2);
	assert(move.second.first == 3);
	assert(move.second.second == 3);
}