#include <iostream>
#include "Maze.h";
using namespace std;

Maze::Maze()
{
	srand((unsigned int) time(NULL));
	NO_CLIP = false;
	BREAK = false;
	TRAIL = SHOW_TRAIL;
	USE_PRIM = false;
}

Maze::Maze(bool type)
{
	srand((unsigned int) time(NULL));
	NO_CLIP = false;
	BREAK = false;
	TRAIL = SHOW_TRAIL;
	USE_PRIM = type;
}

void Maze::Go()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, 0x07); // Reset the text color

	distTraveled = 0;
	gridSize = GetSize();

 	maze = new Cell* [gridSize]; // Building the array of pointers to cells
	for (int i = 0; i < gridSize; i++)
		maze[i] = new Cell [gridSize];

	Initilize();
	//playerPos.Set(random(0, gridSize - 1), random(0, gridSize - 1)); // To start player @ random position
	playerPos.Set(0, 0); // We start @ top-left corner
	portalPos.Set(-1, -1); // We hide the portal so that Generate knows to position the portal
	
	system("cls");
	
	if (USE_PRIM)
		GeneratePrim();
	else
		Generate(playerPos); // Generate the maze from the starting position

	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	/*ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide console window
	ShowWindow(GetConsoleWindow(), SW_SHOW); // Show console window
	MoveWindow(GetConsoleWindow(), 200, 0, 800, 1000, true);*/
	Draw();

	while (true)
	{
		Key key = GetMove(); // Gets a valid move
		if (key == QUIT) // If user wants to quit
		{
			COORD position = {0, gridSize * 2 + 3}; // Set cursor position to under the grid
			SetConsoleCursorPosition(hOut, position);
			SetConsoleTextAttribute(hOut, 0x07); // Reset the text color (was altered by maze)
			cout << "Game Aborted. You moved " << distTraveled << " times.\n\n";
			break;
		}
		else // User did a valid move
		{
			MakeMove(key);
		}
		
		if (playerPos == finishPos) // Reached the end and wins
		{
			COORD position = {0, gridSize * 2 + 3};
			SetConsoleCursorPosition(hOut, position);
			SetConsoleTextAttribute(hOut, 0x07);
			cout << "Congrats! You have won in " << distTraveled << " steps!\n\n";
			break;
		}
	}
	// Overides the standard message
	cout << "Press any key to quit . . . ";
	system("pause > nul");
}

int Maze::GetSize()
{
	int size;

	cout << "Enter width of maze (" << MINSIZE << " to " << MAXSIZE << "): ";
	cin >> size;

	while (size < MINSIZE || size > MAXSIZE)
	{
		cout << "Invalid response, enter width (" << MINSIZE << " to " << MAXSIZE << "): ";
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
	Coord DIRECTIONS[4]; // Lists the possible directions, so we can loop through them
	DIRECTIONS[0].Set(-1, 0);
	DIRECTIONS[1].Set(0, 1);
	DIRECTIONS[2].Set(1, 0);
	DIRECTIONS[3].Set(0, -1);

	int currDist; // Keeps track of the current distance from start (is equal to positions.Size())
	int maxDist = 0; // Keeps track of the longest distance from start (so we place the end point at the right spot)
	Coord currPos = startPos; // Start generating from provided coordinate
	Coord newPos; // Next position to go
	Coord currDirection; // We need to save the current direction in a variable because we need it more than once (moving, breaking the walls)
	Stack positions; // Saves all the positions where we go
	Stack directions(4); // Current possible directions depending on grid boundaries and visited
	bool hidePortal = (portalPos == playerPos); // 1st time we display the portal, but not 2nd time (if player currently is on portal)

	positions.Push(currPos); // Add current position to stack because our loop needs the top position in stack
	while (positions.Size() > 0) // While away from start point
	{
		currPos = positions.Pop(); // Get top position
		positions.Push(currPos); // Put it back
		currDist = positions.Size(); // This is our distance from start point
		maze[currPos.Y][currPos.X].visited = true; // We visited this
		if (currDist > maxDist) // If we are farther than our max
			maxDist = currDist; // Save the new max

		directions.Clear(); // Empty the stack
		for (int count = 0; count < 4; count ++) // Search for all possible directions in directions and add them in stack
		{
			newPos = currPos + DIRECTIONS[count]; // Adding the possible direction to where we are, so that we can check if new position is valid
			if (newPos.X >= 0 && newPos.X < gridSize && newPos.Y >= 0 && newPos.Y < gridSize) // If it is in boundaries of maze
				if (!maze[newPos.Y][newPos.X].visited) // If never visited
					directions.Push(DIRECTIONS[count]); // Add this direction to possible directions
		}

		if (directions.Size() > 0) // If we can go somewhere
		{
			currDirection = directions.GetRandom(); // Get a random direction
			newPos = currPos + currDirection; // Calculate new position

			for (int count = 0; count < 4; count ++) // Loop through all directions (up down left right) and depending on index, we know which way we went
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
			if (currDist == maxDist) // If we just set the max distance to our current distance, then we are the farthest from the start
			{
				portalPos = positions.GetRandom(positions.Size() / 2, (positions.Size() * 3) / 4); // Get a random position between 1/2 and 3/4 of all positions
				finishPos = positions.Pop(); // finish is where we are right now and we remove that spot from stack (we are going back)
			}
			else
				positions.Pop(); // Just go back by removing current position.stop
		}
	}
	if (hidePortal)
	{
		portalPos.Set(-1, -1); // Set it outside of boundaries, so it will not be displayed and therefore not reached
	}
}

void Maze::GeneratePrim()
{
	Coord DIRECTIONS[4]; // Lists the possible directions, so we can loop through them
	DIRECTIONS[0].Set(-1, 0);
	DIRECTIONS[1].Set(0, 1);
	DIRECTIONS[2].Set(1, 0);
	DIRECTIONS[3].Set(0, -1);
	portalPos.Set(-1, -1);
	finishPos.Set(gridSize - 1, gridSize - 1);

	Stack branches;
	Coord currPos(0, 0); // Start generating from provided coordinate
	Coord newPos; // Next position to go
	Coord currDirection; // We need to save the current direction in a variable because we need it more than once (moving, breaking the walls)
	branches.Push(currPos);

	while (branches.Size() > 0) // While away from start point
	{
		branches.Shuffle();
		currPos = branches.Pop(); // Get top position
		maze[currPos.Y][currPos.X].visited = true; // We visited this

		/*portalPos.Set(currPos.Y, currPos.X);
		Draw();
		system("pause");*/

		Stack directions(4); // Current possible directions depending on grid boundaries and visited

		for (int count = 0; count < 4; count ++) // Search for all possible directions and add them in stack
		{
			newPos = currPos + DIRECTIONS[count]; // Adding the possible direction to where we are, so that we can check if new position is valid
			if (newPos.X >= 0 && newPos.X < gridSize && newPos.Y >= 0 && newPos.Y < gridSize) // If it is in boundaries of maze
				if (maze[newPos.Y][newPos.X].visited == false) // If never visited
					directions.Push(DIRECTIONS[count]); // Add this direction to possible directions
		}

		if (directions.Size() > 0) // If we can go somewhere
		{
			directions.Shuffle();
			currDirection = directions.Pop(); // Get a random direction
			newPos = currPos + currDirection; // Calculate new position
			branches.Push(newPos); // Add new position to stack
			maze[newPos.Y][newPos.X].visited = true;
			for (int count = 0; count < 4; count ++) // Loop through all directions (up down left right) and depending on index, we know which way we went
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
					break;
				}
			}
			if (directions.Size() > 1) // If we can still go somewhere else
				branches.Push(currPos); // Save that spot for later
		}
	}
}

void Maze::Draw()
{
	Intersection SINGLE[] = {{' ', false, false, false, false},// (SPACE)
							{218, false, true, true, false},// ┌
							{191, false, false, true, true},// ┐
							{217, true, false, false, true},// ┘
							{192, true, true, false, false},// └
							{196, false, true, false, true},// ─
							{179, true, false, true, false},// │
							{194, false, true, true, true},//  ┬
							{180, true, false, true, true},//  ┤
							{193, true, true, false, true},//  ┴
							{195, true, true, true, false},//  ├
							{197, true, true, true, true}};//  ┼
	Intersection DOUBLE[] = {{' ', false, false, false, false},// (SPACE)
							{201, false, true, true, false},// ╔
							{187, false, false, true, true},// ╗
							{188, true, false, false, true},// ╝
							{200, true, true, false, false},// ╚
							{205, false, true, false, true},// ═
							{186, true, false, true, false},// ║
							{203, false, true, true, true},//  ╦
							{185, true, false, true, true},//  ╣
							{202, true, true, false, true},//  ╩
							{204, true, true, true, false},//  ╠
							{206, true, true, true, true}};//  ╬

	Intersection * style = SINGLE;
	if (DOUBLE_WALLS)
		style = DOUBLE;

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");
	SetConsoleTextAttribute(hOut, WALLS_COLOR);

	for (int row = 0; row <= gridSize * 2; row ++) // Draw Rows
	{
		if (row % 2 == 0) // If row is even, draw border row
		{
			for (int col = 0; col <= gridSize * 2; col ++)
			{
				if (col % 2 == 0) // If col is even, draw connection
				{
					// Depending on the cell position, we only check the connections that matter (we can't check outside of boundaries)

					Intersection requirements = {' ', false, false, false, false}; // Start with no connection
					
					if (row == 0) // Top row
					{
						if (col == 0) // Leftmost column
						{
							requirements.B = maze[row / 2][col / 2].left;
							requirements.R = maze[row / 2][col / 2].top;
						}
						else if (col == gridSize * 2) // Rightmost column
						{
							requirements.B = maze[row / 2][col / 2 - 1].right;
							requirements.L = maze[row / 2][col / 2 - 1].top;
						}
						else // anything in between
						{
							requirements.B = maze[row / 2][col / 2].left;
							requirements.L = maze[row / 2][col / 2 - 1].top;
							requirements.R = maze[row / 2][col / 2].top;
						}
					}
					else if (row == gridSize * 2) // Bottom row
					{
						if (col == 0) // Leftmost column
						{
							requirements.T = maze[row / 2 - 1][col / 2].left;
							requirements.R = maze[row / 2 - 1][col / 2].bottom;
						}
						else if (col == gridSize * 2) // Rightmost column
						{
							requirements.T = maze[row / 2 - 1][col / 2 - 1].right;
							requirements.L = maze[row / 2 - 1][col / 2 - 1].bottom;
						}
						else // anything in between
						{
							requirements.T = maze[row / 2 - 1][col / 2].left;
							requirements.L = maze[row / 2 - 1][col / 2 - 1].bottom;
							requirements.R = maze[row / 2 - 1][col / 2].bottom;
						}
					}
					else // anything in between
					{
						if (col == 0) // Leftmost column
						{
							requirements.T = maze[row / 2 - 1][col / 2].left;
							requirements.B = maze[row / 2][col / 2].left;
							requirements.R = maze[row / 2][col / 2].top;
						}
						else if (col == gridSize * 2) // Rightmost column
						{
							requirements.T = maze[row / 2 - 1][col / 2 - 1].right;
							requirements.B = maze[row / 2][col / 2 - 1].right;
							requirements.L = maze[row / 2][col / 2 - 1].top;
						}
						else // anything in between
						{
							requirements.T = maze[row / 2 - 1][col / 2].left;
							requirements.L = maze[row / 2][col / 2 - 1].top;
							requirements.B = maze[row / 2][col / 2].left;
							requirements.R = maze[row / 2][col / 2].top;
						}
					}

					int type;
					for (type = 0; type < 12; type ++) // Search in the preset intersections according to our requirements
						if (requirements.T == style[type].T &&
							requirements.R == style[type].R &&
							requirements.B == style[type].B &&
							requirements.L == style[type].L)
							break;
					if (type == 12) // If it was not found in array of preset intersections
						cout << ' ';
					else
						cout << style[type].C; // Display the correct intersection
				}
				else // col is odd, draw horizontal line
				{
					if (row == gridSize * 2) // If bottom row, look the cell above
					{
						if (maze[row / 2 - 1][col / 2].bottom)
							cout << style[5].C;
						else
							cout << ' ';
					}
					else
					{
						if (maze[row / 2][col / 2].top) // Look at current cell, which is under that line
							cout << style[5].C;
						else
							cout << ' ';
					}
				}
			}
		}
		else // row is odd, draw content row
		{
			for (int col = 0; col <= gridSize * 2; col ++)
			{
				if (col % 2 == 0) // If col is even, draw vertical line
				{
					if (col == gridSize * 2) // If rightmost col, look left
					{
						if (maze[row / 2][col / 2 - 1].right)
							cout << style[6].C;
						else
							cout << ' ';
					}
					else
					{
						if (maze[row / 2][col / 2].left) // Look at current cell, which is to the right
							cout << style[6].C;
						else
							cout << ' ';
					}
				}
				else // col is odd, draw content
				{
					Coord currPos(row / 2, col / 2); // Save current position into a Coord, so that we can compare using operator== (instead of .Y==row && .X==col)
					if (currPos == playerPos)
					{
						SetConsoleTextAttribute(hOut, PLAYER_COLOR); // Change color
						cout << PLAYER_CHAR;
					}
					else if (currPos == portalPos)
					{
						SetConsoleTextAttribute(hOut, PORTAL_COLOR); // Change color
						cout << PORTAL_CHAR;
					}
					else if (currPos == finishPos)
					{
						SetConsoleTextAttribute(hOut, FINISH_COLOR); // Change color
						cout << FINISH_CHAR;
					}/*
					else if (maze[currPos.Y][currPos.X].visited)
						cout << 'V';*/
					else
						cout << EMPTY_CHAR;

					SetConsoleTextAttribute(hOut, WALLS_COLOR); // Reset color for the walls
				}
			}
		}
		cout << endl; // This line is done
	}
	cout << "Use the arrow keys to move, Q to quit";
}

Key Maze::GetMove()
{
	do
	{
		int key = getch(); // Wait for keyboard input
		if (key == 81) // Q becomes q
			key = 113;

		if (key == QUIT)
			return QUIT;
		if ((key == 110 || key == 78) && NO_CLIP_ENABLED) // 'n' or 'N' and cheat is usable, switch on/off
			NO_CLIP ^= true;
		if ((key == 98 || key == 66) && BREAK_ENABLED) // 'b' or 'B' and cheat is usable, switch on/off
		{
			BREAK ^= true;
			NO_CLIP = BREAK;
		}
		if (key == 116 || key == 84) // 't' or 'T'
			TRAIL ^= true;
		
		if ((key == RIGHT || key == LEFT || key == UP || key == DOWN) && (NO_CLIP || BREAK)) // If NO_CLIP and arrow key, then we don't care about walls
			return (Key) key;
		if (key == RIGHT && maze[playerPos.Y][playerPos.X].right == false) // If there is no wall
			return RIGHT;
		if (key == LEFT && maze[playerPos.Y][playerPos.X].left == false)
			return LEFT;
		if (key == UP && maze[playerPos.Y][playerPos.X].top == false)
			return UP;
		if (key == DOWN && maze[playerPos.Y][playerPos.X].bottom == false)
			return DOWN;
	}
	while(true); // Will only exit is something is being returned (valid key press)
}

void Maze::MakeMove(Key key)
{
	COORD position = {(playerPos.X * 2) + 1, (playerPos.Y * 2) + 1}; // Going from position in grid to position in console (takes into account the borders)
	
	// Removes the flashing cursor
	CONSOLE_CURSOR_INFO cciInfo;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cciInfo.dwSize = 1;
    cciInfo.bVisible = false;   
    SetConsoleCursorInfo(hOut, &cciInfo);     

	distTraveled++;

	SetConsoleCursorPosition(hOut, position);
	if (TRAIL)
	{
		SetConsoleTextAttribute(hOut, TRAIL_COLOR); // Set color for breadcrumbs
		cout << TRAIL_CHAR; // Print breadcrumb over player's old position
	}
	else
		cout << EMPTY_CHAR;

	if (BREAK)
	{
		position.X = (playerPos.X * 2) + 1; // From position in grid to position in console
		position.Y = (playerPos.Y * 2) + 1;
		if (key == RIGHT)
		{
			position.X += 1;
			maze[playerPos.Y][playerPos.X].right = false;
			maze[playerPos.Y][playerPos.X + 1].left = false;
		}
		else if (key == LEFT)
		{
			position.X -= 1;
			maze[playerPos.Y][playerPos.X].left = false;
			maze[playerPos.Y][playerPos.X - 1].right = false;
		}
		else if (key == UP)
		{
			position.Y -= 1;
			maze[playerPos.Y][playerPos.X].top = false;
			maze[playerPos.Y - 1][playerPos.X].bottom = false;
		}
		else //key == DOWN
		{
			position.Y += 1;
			maze[playerPos.Y][playerPos.X].bottom = false;
			maze[playerPos.Y + 1][playerPos.X].top = false;
		}
		SetConsoleCursorPosition(hOut, position);
		cout << EMPTY_CHAR;
	}

	// We move the player:
	if (key == RIGHT)
		playerPos.X += 1;
	else if (key == LEFT)
		playerPos.X -= 1;
	else if (key == UP)
		playerPos.Y -= 1;
	else //key == DOWN
		playerPos.Y += 1;

	position.X = (playerPos.X * 2) + 1; // From position in grid to position in console
	position.Y = (playerPos.Y * 2) + 1;
	SetConsoleTextAttribute(hOut, PLAYER_COLOR); // Set color for player
	SetConsoleCursorPosition(hOut, position);
	cout << PLAYER_CHAR; // Print the player character to the new position

	if (playerPos == portalPos) // If player has reached the portal
	{
		Initilize(); // Reset cell array
		Generate(portalPos); // Remap the maze starting at the portalPos
		Draw();
	}
}