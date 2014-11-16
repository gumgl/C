#ifndef _MAZE_
#define _MAZE_

#include "stack.h"

struct Cell
{
	bool top;
	bool right;
	bool bottom;
	bool left;
	bool visited;
};

struct Intersection
{
	char C;//ASCII Character
	bool T, R, B, L;//Top, Right, Bottom, Left
};

//	Game
const int  BACKGROUND_COLOR = 0x00;
const char EMPTY_CHAR = ' ';
const int  WALLS_COLOR = 0x07 + BACKGROUND_COLOR;
//	Trail
const char TRAIL_CHAR = 249; // ∙ 
const int  TRAIL_COLOR = 0x0E + BACKGROUND_COLOR;
//	Player
const char PLAYER_CHAR = 'X';
const int  PLAYER_COLOR = 0x0A + BACKGROUND_COLOR;
//	Portal
const char PORTAL_CHAR = 'O';
const int  PORTAL_COLOR = 0x0B + BACKGROUND_COLOR;
//	Finish
const char FINISH_CHAR = 'F';
const int  FINISH_COLOR = 0x0C + BACKGROUND_COLOR;

enum Key {QUIT = 113, UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77}; // KeyCodes
const int  MINSIZE = 5;
const int  MAXSIZE = 15;
const bool SHOW_TRAIL = true;
const bool DOUBLE_WALLS = false; // ┼ or ╬
const bool NO_CLIP_ENABLED = true; // Cheat: can go through walls
const bool BREAK_ENABLED = true; // Cheat: can break the walls

class Maze
{
	public:
		Maze();
		void Go(); // Handles all the gameplay and calls everything else
	private:
		Coord playerPos;
		Coord portalPos;
		Coord finishPos;
		int distTraveled;
		int gridSize;
		bool NO_CLIP;
		bool BREAK;
		Cell ** maze; // Declares a 2-dimensional pointer (pointer to a pointer)

		int GetSize(); // Gets valid user input for size
		void Initilize(); // Resets all cells in array
		void Generate(Coord); // Sends startPos of player
		void GeneratePrim(); // Generates the maze using Prim's algorithm
		void Draw(); // Draws the maze
		Key  GetMove(); // Gets a valid move and return the key pressed
		void MakeMove(Key); // Moves the player
};

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

#endif