#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Maze.h";
using namespace std;

Maze::Maze()
{
	srand ( (unsigned int)time(NULL) );

	distTraveled = 0;
}

void Maze::Go()
{
	gridSize = GetSize();

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

 	maze = new Cell* [gridSize];
	for (int i = 0; i < gridSize; i++)
		maze[i] = new Cell [gridSize];

	Initilize();
	playerPos.Set(random(0, gridSize - 1), random(0, gridSize - 1));
	portalPos.Set(-1, -1);
	maze[playerPos.Y][playerPos.X].content = PLAYER;
	
	system("cls");
	
	Generate(playerPos);

	Draw();

	while(true)
	{
		Key key = GetMove();
		if (key != QUIT)
		{
			MakeMove(key);
			//Draw();
		}
		if (key == QUIT)
		{
			COORD position = {0, gridSize * 2 + 1};
			SetConsoleCursorPosition(hOut, position);
			cout << "Game Aborted" << endl;
			break;
		}
		else if (playerPos == finishPos)
		{
			COORD position = {0, gridSize * 2 + 1};
			SetConsoleCursorPosition(hOut, position);
			cout << "Congrats, You have won in " << distTraveled << " steps!" << endl;
			break;
		}
	}
}

int Maze::GetSize()
{
	int size;

	cout << "Enter width of maze (" << MINSIZE << " to " << MAXSIZE << "): ";
	cin >> size;

	while (size < MINSIZE || size > MAXSIZE)
	{
		cout << "Invalid response, Enter width (" << MINSIZE << " to " << MAXSIZE << "): ";
		cin >> size;
	}

	return size;
}

void Maze::Initilize()
{
	int row;
	int col;

	for (row = 0; row < gridSize; row++)
	{
		for (col = 0; col < gridSize; col++)
		{
			maze[row][col].content = NOTHING;
			maze[row][col].top = true;
			maze[row][col].right = true;
			maze[row][col].bottom = true;
			maze[row][col].left = true;
			maze[row][col].visited = false;
		}
	}
}

void Maze::Generate(Coord startPos)
{
	Coord DIRECTIONS[4];
	DIRECTIONS[0].Set(-1, 0);
	DIRECTIONS[1].Set(0, 1);
	DIRECTIONS[2].Set(1, 0);
	DIRECTIONS[3].Set(0, -1);

	int currDist;
	int maxDist = 0;
	Coord currPos = startPos;
	Coord newPos;
	Coord currDirection;
	Stack positions;
	Stack directions(4);
	bool hidePortal = (portalPos == playerPos);

	positions.Push(currPos);
	while (positions.Size() > 0) // While away from start point
	{
		currPos = positions.Pop(); // Get top position
		positions.Push(currPos); // Put it back
		currDist = positions.Size(); // This is our distance from start point
		maze[currPos.Y][currPos.X].visited = true; // We visited this
		if (currDist > maxDist)
			maxDist = currDist;

		directions.Clear();
		for (int count = 0; count < 4; count ++) // Add all possible directions in directions[]
		{
			newPos = currPos + DIRECTIONS[count];
			if (newPos.X >= 0 && newPos.X < gridSize && newPos.Y >= 0 && newPos.Y < gridSize) // If it is in boundaries of maze
				if (!maze[newPos.Y][newPos.X].visited) // If never visited
					directions.Push(DIRECTIONS[count]); // Add this direction to possible directions
		}

		if (directions.Size() > 0) // If we can go somewhere
		{
			currDirection = directions.GetRandom(); // Get a random direction
			newPos = currPos + currDirection; // Calculate new position

			for (int count = 0; count < 4; count ++) // BREAK THE WALLS
			{
				if (currDirection == DIRECTIONS[count])
				{
					switch (count)
					{
					case 0: // going up
						maze[currPos.Y][currPos.X].top = false;
						maze[newPos.Y][newPos.X].bottom = false;
						break;
					case 1: // going right
						maze[currPos.Y][currPos.X].right = false;
						maze[newPos.Y][newPos.X].left = false;
						break;
					case 2: // going down
						maze[currPos.Y][currPos.X].bottom = false;
						maze[newPos.Y][newPos.X].top = false;
						break;
					case 3: // going left
						maze[currPos.Y][currPos.X].left = false;
						maze[newPos.Y][newPos.X].right = false;
						break;
					}
				}
			}
			positions.Push(newPos); // Add new position to stack
		}
		else // no possible direction, go back
		{
			if (currDist == maxDist)
			{
				portalPos = positions.GetRandom(positions.Size() / 2, positions.Size() - 2);
				finishPos = positions.Pop();
			}
			else
				positions.Pop();
		}
	}
	maze[finishPos.Y][finishPos.X].content = FINISH;
	if (hidePortal)
	{
		portalPos.Set(-1, -1);
	}
	else
		maze[portalPos.Y][portalPos.X].content = PORTAL;
}

void Maze::Draw()
{
	int col;
	int row;
	char vLine = 179;
	char hLine = 196;

	system("cls");

	for (row = 0; row < gridSize; row++)
	{
		for (col = 0; col < gridSize; col++)
		{
			cout << "+";

			if (maze[row][col].top == true)
				cout << hLine;
			else
				cout << " ";
		}
		cout << "+" << endl;

		for (col = 0; col < gridSize; col++)
		{
			if (maze[row][col].left == true)
				cout << vLine;
			else
				cout << " ";
			
			cout << maze[row][col].content;
		}
		cout << vLine << endl;
	}

	for (col = 0; col < gridSize; col++)
	{
			cout << "+" << hLine;
	}
	cout << "+" << endl;
}

Key Maze::GetMove()
{
	int key;

	key = getch();
	if (key == 81) // Q becomes q
		key = 113;

	if (key == QUIT)
		return QUIT;
	if (key == RIGHT && maze[playerPos.Y][playerPos.X].right == false)
		return RIGHT;
	if (key == LEFT && maze[playerPos.Y][playerPos.X].left == false)
		return LEFT;
	if (key == UP && maze[playerPos.Y][playerPos.X].top == false)
		return UP;
	if (key == DOWN && maze[playerPos.Y][playerPos.X].bottom == false)
		return DOWN;

	return GetMove();
}

void Maze::MakeMove(Key key)
{
	COORD position;
	
	CONSOLE_CURSOR_INFO cciInfo;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    cciInfo.dwSize = 1;
    cciInfo.bVisible = false;   
     
    SetConsoleCursorInfo(hOut, &cciInfo);     

	distTraveled++;
	maze[playerPos.Y][playerPos.X].content = TRAIL; // leave a trail

	position.X = (playerPos.X * 2) + 1;
	position.Y = (playerPos.Y * 2) + 1;
	SetConsoleCursorPosition(hOut, position);
	cout << TRAIL;

	if (key == RIGHT)
		playerPos.X += 1;
	else if (key == LEFT)
		playerPos.X -= 1;
	else if (key == UP)
		playerPos.Y -= 1;
	else //key == DOWN
		playerPos.Y += 1;
	
	maze[playerPos.Y][playerPos.X].content = PLAYER;

	position.X = (playerPos.X * 2) + 1;
	position.Y = (playerPos.Y * 2) + 1;
	SetConsoleCursorPosition(hOut, position);
	cout << PLAYER;
	SetConsoleCursorPosition(hOut, position);

	if (playerPos == portalPos) //player has reached the end
	{
		Initilize(); //reset cell array
		Generate(portalPos); //remap the maze starting at the portalPos
		Draw();
		position.X = (playerPos.X * 2) + 1;
		position.Y = (playerPos.Y * 2) + 1;
		SetConsoleCursorPosition(hOut, position);
		cout << PLAYER;
		SetConsoleCursorPosition(hOut, position);
		maze[playerPos.Y][playerPos.X].content = PLAYER;
	}
}