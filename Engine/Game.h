/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Surface.h"
#include "Piece.h"
#include "Board.h"
#include <vector>

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	//Draw
	void ComposeFrame();
	void drawMainMenu();
	void drawGameScreen();
	void drawLoadScreen();
	//Logic
	void UpdateModel();
	void mainMenuLogic();
	void singlePlayerLogic();
	void multiPlayerLogic();
	void loadGameLogic();
	/********************************/
	/*  User Functions              */
	//Drawing stuff
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	const int squareSize = 75;
	const std::string GAMEFILE = "game.pgn";
	const Color CHROMA = Color(254, 254, 195);

	bool gameIsLoaded = false;
	std::string currentScreen = "menu";
	int result = 2;

	std::vector<std::pair<int, int>> toHighlight;

	Surface highlighter = Surface("assets/highlight.bmp");
	Surface menuScreen = Surface("assets/menu.bmp");
	Surface nextButton = Surface("assets/next.bmp");
	Surface prevButton = Surface("assets/prev.bmp");
	Surface exitText = Surface("assets/exitText300x100.bmp");
	Surface whiteWon = Surface("assets/whiteWon.bmp");
	Surface blackWon = Surface("assets/blackWon.bmp");
	Surface draw = Surface("assets/draw.bmp");

	bool pieceIsSelected = false;
	int selectedX = 0, selectedY = 0;
	
	Board board;
	/********************************/
};