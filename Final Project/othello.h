#ifndef _OTHELLO_
#define _OTHELLO_

#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h> // For Console Title
#include "stack.h"

enum boardPiece {Blank, White, Black};

// --Settings--
const int BOARD_SIZE = 8; // If not 8, will create bugs with AI
const char PLAYER_TOKEN = char(223); // ▀ (character will change color)
const bool HIGHLIGHT_VALID_MOVES = false; // Highlight the valid moves
const bool INSTANT_AI = true; // Play AI turns directly. Otherwise, will wait for user input
const boardPiece START_PLAYER = White;

class Othello
{
	private:
		int gameType; // User setting. 0 = No AI; 1 = Computer plays any valid move; 2 = Computer looks 1 move ahead; 3 = Computer looks 6 moves ahead 
		boardPiece theBoard[BOARD_SIZE][BOARD_SIZE];
		boardPiece currentPlayer;

		void Init();
		bool Draw(); // Draws the board and returns true if there are possible moves
		bool ValidMove(int, int);
		void NegaMax();
		int  Max(int);
		int  Evaluate();
		int  MoveValue(int, int, int = 1);
		bool MakeMove(int, int);
		void MakeAIMove();
		void GetMove();
		void SwitchPlayer();
		void EndOfGame();
	public:
		Othello();
		Othello(int); // gameType parameter
		void Play();
};

#endif