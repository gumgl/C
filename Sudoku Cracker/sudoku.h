#ifndef _SUDOKU_
#define _SUDOKU_

#include "stack.h"
#include <string>
#include <conio.h>   // Mouse Events
#include <Windows.h> // Colors & Cursor position
#include <ctime>     // Time
#include <tchar.h>   // Console Title

const int HIGHLIGHT_COLOR = 0xB0; // Color of the highlighted cell when user clicks it
const char REVEAL_CHAR = '?'; // Reveal symbol
const int REVEAL_COLOR = 0xFC; // Color of the reveal symbol (i.e. "?")
const int REVEALED_COLOR = 0xF2; // Color of the revealed number
const int ERROR_TIME = 4; // Time (in seconds) allowed to solve the grid

class Sudoku
{
	private:
		int grid[9][9];
		bool empty[9][9];
		COORD GetClick(); // Gets a position from user input (mouse click)
		void Display(); // Draws the grid with current numbers
		int GetNumber(); // Gets a number from user input
		int CountPossibleDigits(Coord); // Counts the number of possible digits for a cell
		int GetDigit(Coord); // Returns the first possible digit for a cell
		bool IsValid(COORD, int); // Tells you if a number is valid somewhere in the grid
		void MoveForward(COORD&); // Moves forward to the next editable cell in the grid
		void MoveBackward(COORD&); // Moves backward to the previous editable cell in the grid
	public:
		Sudoku();
		void GetNumbers(); // Gets the numbers before solving the sudoku
		void Solve(); // Fills up the grid with valid digits
		void ShowSolution(); // Displays solution depending on user preference
		bool error;
};

#endif

//	--- COLOR INFO ---
//	0xAB where A = background, B = foreground
//	
//	0 = Black
//	1 = Blue
//	2 = Green
//	3 = Aqua
//	4 = Red
//	5 = Purple
//	6 = Yellow
//	7 = White
//	8 = Gray
//	9 = Light Blue
//	A = Light Green
//	B = Light Aqua
//	C = Light Red
//	D = Light Purple
//	E = Light Yellow
//	F = Bright White
//	
//	For color examples: http://en.wikipedia.org/wiki/ANSI_escape_code#Colors