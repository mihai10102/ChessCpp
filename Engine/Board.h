#pragma once

#include "Piece.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"
#include "nullPiece.h"
#include "pieceValidator.h"
#include <vector>

class Board{
private:
	//Piece movement
	bool whiteCanCastleKingSide = false;
	bool whiteCanCastleQueenSide = false;
	bool blackCanCastleKingSide = false;
	bool blackCanCastleQueenSide = false;

	pieceValidator validator;
public:
	Piece* board[64] = {};

	//Game controls
	bool whiteToMove = true;
	bool gameOver = false;

	//Loading games from files
	std::vector<std::string> game;
	int currentGameMove = -1;
public:
	Board();
	~Board();

	Board& operator=(const Board& b);

	//Move making
	std::vector<int> getLegalMoves(int x, int y);
	void makeMove(int oldX, int oldY, int newX, int newY);
	bool moveIsLegal(int oldX, int oldY, int newX, int newY);
	bool kingCanBeCaptured(int kingColor, Piece* board[64]);

	//Castling
	std::vector<int> getSquaresAttackedBySide(int side);
	void setCastlingOptions();

	//Bot stuff
	void makeRandomMove(int side);

	//Usefull for many things
	bool sideHasLegalMoves(int side);

	//End the match
	int getMatchResult();
	bool drawByInsufficientMaterial();
	bool drawByRepetition();
	
	//Load game
	void resetBoard();
	std::vector<std::string> getGameFromFile(std::string filename);
	std::pair<std::pair<int, int>, std::pair<int, int>> convertStandardNotationToMove(std::string move);
	std::string convertBoardToString();
	void loadBoardFromString(std::string);
	void getPrevPosition();
	void getNextPosition();
};