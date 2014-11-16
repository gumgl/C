#ifndef _OTHELLO_
#define _OTHELLO_

#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h> // For Console Title
#include "stack.h"

enum boardPiece {Blank, White, Black};

// --Settings--
const int BOARD_SIZE = 8;
const char PLAYER_TOKEN = char(223); // ▀ (character will change color)
const bool HIGHLIGHT_VALID_MOVES = false; // Highlight the valid moves
const bool ENABLE_AI = true; // Ask user to choose Game Type
const bool INSTANT_AI = true; // Play AI turns directly. Otherwise, will wait for user input
const boardPiece START_PLAYER = White;

class Othello
{
	private:
		int gameType; // User setting. 0 = No AI, 1 = Computer plays any valid move, 2 = Computer plays the best move, 3 = Computer plays the best move
		boardPiece theBoard[BOARD_SIZE][BOARD_SIZE];
		boardPiece currentPlayer;

		void Init();
		bool Draw(); // Draws the board and returns true if there are possible moves
		bool ValidMove(int, int);
		void MiniMax();
		int  Max(int);
		int  Evaluate();
		int  MoveValue(int, int, int = 1);
		void MakeMove(int, int);
		void MakeAIMove();
		void GetMove();
		void SwitchPlayer();
		void EndOfGame();
	public:
		Othello();
		void Play();
};

#endif