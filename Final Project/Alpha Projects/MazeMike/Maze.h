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
	char content;
};

enum Key {QUIT = 113, UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77};
const char TRAIL = 249; // ∙ 
const char PLAYER = 'X';
const char PORTAL = 'O';
const char FINISH = 'F';
const char NOTHING = ' ';
const int MINSIZE = 10;
const int MAXSIZE = 15;

class Maze
{
	private:
		Coord playerPos;
		Coord portalPos;
		Coord finishPos;
		int distTraveled;
		int gridSize;
		Cell ** maze;

		int GetSize();
		void Initilize();
		void Generate(Coord); //Sends startPos of player
		void Draw();
		Key GetMove();
		void MakeMove(Key);
	public:
		Maze();
		void Go();
};

#endif