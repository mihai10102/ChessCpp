/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Tests.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	
	//testPawn();
	//testRook();
	//testBishop();
	//testKnight();
	//testQueen();
	//testKing();
	//testBoard();

}

void Game::Go(){
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

//Main Loop Components
void Game::UpdateModel(){
	if (currentScreen == "menu") {
		mainMenuLogic();
	}
	else if (currentScreen == "single") {
		singlePlayerLogic();
	}
	else if (currentScreen == "multiplayer") {
		multiPlayerLogic();
	}
	else if (currentScreen == "load") {
		loadGameLogic();
	}
}
void Game::ComposeFrame(){
	if (currentScreen == "menu") {
		drawMainMenu();
	}
	else if (currentScreen == "single" || currentScreen == "multiplayer") {
		drawGameScreen();
	}
	else if (currentScreen == "load") {
		drawLoadScreen();
	}
}

//Draw
void Game::drawMainMenu(){
	gfx.drawSurface(0, 0, menuScreen, CHROMA);
}
void Game::drawGameScreen(){
	gfx.drawChessBoard(squareSize);

	for (int i = 0; i < 64; i++) {
		if (!(board.board[i]->isNull())) {
			gfx.drawSurface(board.board[i]->x*squareSize, board.board[i]->y*squareSize, board.board[i]->img, CHROMA);
		}
	}

	for (size_t i = 0; i < toHighlight.size(); i++) {
		gfx.drawSurface(toHighlight[i].first*squareSize, toHighlight[i].second*squareSize, highlighter, CHROMA);
	}

	if (board.gameOver) {
		//Show result
		if (result == 1) {
			gfx.drawSurface(650, 50, whiteWon, CHROMA);
		}
		else if (result == -1) {
			gfx.drawSurface(650, 50, blackWon, CHROMA);
		}
		else if (result == 0) {
			gfx.drawSurface(650, 50, draw, CHROMA);
		}

		//Exit text
		gfx.drawSurface(650, 200, exitText, CHROMA);
	}
}
void Game::drawLoadScreen() {
	drawGameScreen();

	//Exit text
	gfx.drawSurface(650, 100, exitText, CHROMA);

	//Buttons
	gfx.drawSurface(650, 400, prevButton, CHROMA);
	gfx.drawSurface(850, 410, nextButton, CHROMA);
}

//Logic
void Game::mainMenuLogic() {
	if (wnd.mouse.LeftIsPressed()) {
		int mouseX = wnd.mouse.GetPosX();
		int mouseY = wnd.mouse.GetPosY();

		if (mouseY >= 445 && mouseY <= 510) {
			if (mouseX >= 75 && mouseX <= 300) {
				currentScreen = "single";
			}
			if (mouseX >= 385 && mouseX <= 625) {
				currentScreen = "multiplayer";
			}
			if (mouseX >= 700 && mouseX <= 920) {
				currentScreen = "load";
			}
		}
	}
}
void Game::singlePlayerLogic() {
	if (!board.gameOver) {
		if (wnd.mouse.LeftIsPressed()) {

			int x = (wnd.mouse.GetPosX() - wnd.mouse.GetPosX() % squareSize) / squareSize;
			int y = (wnd.mouse.GetPosY() - wnd.mouse.GetPosY() % squareSize) / squareSize;

			if (!pieceIsSelected) {
				std::vector<int> legalMoves = board.getLegalMoves(x, y);
				if (!(board.board[x + y * 8]->isNull())) { pieceIsSelected = true; }
				selectedX = x;
				selectedY = y;
				toHighlight.clear();
				for (size_t i = 0; i < legalMoves.size(); i++) {
					int pos = x + y * 8 + legalMoves[i];
					int newX = pos % 8;
					int newY = (int)(pos / 8);
					std::pair<int, int> move(newX, newY);
					toHighlight.push_back(move);
				}
			}
			else {
				for (size_t i = 0; i < toHighlight.size(); i++) {
					if (toHighlight[i].first == x && toHighlight[i].second == y) {
						board.makeMove(selectedX, selectedY, x, y);
						result = board.getMatchResult();
						if (result == 2) {
							board.makeRandomMove(0);
							result = board.getMatchResult();
							if (result != 2) {
								board.gameOver = true;
							}
						}
						else {
							board.gameOver = true;
						}
					}
				}
				pieceIsSelected = false;
				toHighlight.clear();
			}
		}
	}
	else {
		if (wnd.kbd.KeyIsPressed(VK_ESCAPE)) {
			currentScreen = "menu";
			board.resetBoard();
			board.gameOver = false;
			board.game.clear();
			board.whiteToMove = true;
		}
	}
}
void Game::multiPlayerLogic() {
	if (!board.gameOver) {
		if (wnd.mouse.LeftIsPressed()) {

			int x = (wnd.mouse.GetPosX() - wnd.mouse.GetPosX() % squareSize) / squareSize;
			int y = (wnd.mouse.GetPosY() - wnd.mouse.GetPosY() % squareSize) / squareSize;

			if (!pieceIsSelected) {
				std::vector<int> legalMoves = board.getLegalMoves(x, y);
				if (!(board.board[x + y * 8]->isNull())) { pieceIsSelected = true; }
				selectedX = x;
				selectedY = y;
				toHighlight.clear();
				for (size_t i = 0; i < legalMoves.size(); i++) {
					int pos = x + y * 8 + legalMoves[i];
					int newX = pos % 8;
					int newY = (int)(pos / 8);
					std::pair<int, int> move(newX, newY);
					toHighlight.push_back(move);
				}
			}
			else {
				for (size_t i = 0; i < toHighlight.size(); i++) {
					if (toHighlight[i].first == x && toHighlight[i].second == y) {
						board.makeMove(selectedX, selectedY, x, y);
						result = board.getMatchResult();
						if (result != 2) {
							board.gameOver = true;
						}
					}
				}
				pieceIsSelected = false;
				toHighlight.clear();
			}
		}
	}
	else {
		//Show result
		if (result == 1) {
			gfx.drawSurface(650, 50, whiteWon, CHROMA);
		}
		else if (result == -1) {
			gfx.drawSurface(650, 50, blackWon, CHROMA);
		}

		//Exit text
		gfx.drawSurface(650, 200, exitText, CHROMA);

		if (wnd.kbd.KeyIsPressed(VK_ESCAPE)) {
			currentScreen = "menu";
			board.resetBoard();
			board.gameOver = false;
			board.game.clear();
			board.whiteToMove = true;
		}
	}
}
void Game::loadGameLogic() {
	if (!gameIsLoaded) {
		board.game = board.getGameFromFile(GAMEFILE);
		gameIsLoaded = true;
	}
	else {
		if (wnd.mouse.LeftIsPressed()) {
			if (wnd.mouse.GetPosY() >= 400 && wnd.mouse.GetPosY() <= 500) {
				if (wnd.mouse.GetPosX() >= 650 && wnd.mouse.GetPosX() <= 750) {
					board.getPrevPosition();
				}
				else if (wnd.mouse.GetPosX() >= 850 && wnd.mouse.GetPosX() <= 950) {
					board.getNextPosition();
				}
			}
		}
		if (wnd.kbd.KeyIsPressed(VK_ESCAPE)) {
			currentScreen = "menu";
			board.resetBoard();
			board.gameOver = false;
			board.game.clear();
			board.whiteToMove = true;
		}
	}
}