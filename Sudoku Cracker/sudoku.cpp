#include "sudoku.h"
#include <iostream>

using namespace std;

Sudoku::Sudoku()
{
	srand((unsigned int) time(NULL));
	system("color F0");
	SetConsoleTitleW(_T("Sudoku Solver by Guillaume L.")); // Change Console Title
	for (int row = 0; row < 9; row ++)
		for (int col = 0; col < 9; col ++)
		{
			grid[row][col] = 0;
			empty[row][col] = false;
		}
	error = false;
}

void Sudoku::Display()
{
	system("cls");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << char(218) << char(196) << char(194) << char(196) << char(194) << char(196) << char(194) << char(196) << char(194) << char(196) << char(194) << char(196) << char(194) << char(196) << char(194) << char(196) << char(194) << char(196) << char(191) << endl;
	for (int row = 0; row < 9; row ++)
	{
		cout << char(179); // │
		for (int col = 0; col < 9; col ++)
		{
			if (grid[row][col] > 0)
			{
				if (empty[row][col])
					SetConsoleTextAttribute(hOut, REVEALED_COLOR); // Change color of the revealed digits
				cout << grid[row][col]; // Draw digit
				SetConsoleTextAttribute(hOut, 0xF0); // Reset color to black
			}
			else
			{
				//HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
				//SetConsoleTextAttribute(hOut, 0xF8);
				//cout << CountPossibleDigits(Coord(row, col));
				////cout << GetDigit(Coord(row, col));
				//SetConsoleTextAttribute(hOut, 0xF0);
				cout << " ";
			}
			if (col % 3 == 2)
				SetConsoleTextAttribute(hOut, 0xF0); // Draw black line (box outline)
			else
				SetConsoleTextAttribute(hOut, 0xF7); // Draw gray line

			cout << char(179); // │
			SetConsoleTextAttribute(hOut, 0xF0); // Reset color to black
		}
		cout << endl;
		if (row < 8)
		{
			if (row % 3 == 2) // Then draw black line (box outline)
				cout << char(195) << char(196) << char(197) << char(196) << char(197) << char(196) << char(197) << char(196) << char(197) << char(196) << char(197) << char(196) << char(197) << char(196) << char(197) << char(196) << char(197) << char(196) << char(180) << endl;
			else // Then draw gray lines
			{
				SetConsoleTextAttribute(hOut, 0xF0); // Draw black line (box outline)
				cout << char(195);
				SetConsoleTextAttribute(hOut, 0xF7); // Draw gray line
				cout << char(196) << char(197) << char(196) << char(197) << char(196);
				SetConsoleTextAttribute(hOut, 0xF0); // Draw black line (box outline)
				cout << char(197);
				SetConsoleTextAttribute(hOut, 0xF7); // Draw gray line
				cout << char(196) << char(197) << char(196) << char(197) << char(196);
				SetConsoleTextAttribute(hOut, 0xF0); // Draw black line (box outline)
				cout << char(197);
				SetConsoleTextAttribute(hOut, 0xF7); // Draw gray line
				cout << char(196) << char(197) << char(196) << char(197) << char(196);
				SetConsoleTextAttribute(hOut, 0xF0); // Draw black line (box outline)
				cout << char(180) << endl;
			}
		}
		}
	cout << char(192) << char(196) << char(193) << char(196) << char(193) << char(196) << char(193) << char(196) << char(193) << char(196) << char(193) << char(196) << char(193) << char(196) << char(193) << char(196) << char(193) << char(196) << char(217) << endl;
}

COORD Sudoku::GetClick()
{
	COORD click;
	CONSOLE_CURSOR_INFO cciInfo;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cciInfo.dwSize = 1;
    cciInfo.bVisible = false;   
    SetConsoleCursorInfo(hOut, &cciInfo); 

	

	HANDLE hStdin;
    DWORD cNumRead, fdwMode, fdwSaveOldMode, i;
    INPUT_RECORD irInBuf[128];
    int counter=0;
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    SetConsoleMode(hStdin, fdwMode);

	while(true)
	{
		ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead);

		for (i = 0; i < cNumRead; i++) 
        {
			if (irInBuf[i].EventType == MOUSE_EVENT && irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				click = irInBuf[i].Event.MouseEvent.dwMousePosition;
				break;
			}
			if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.wVirtualKeyCode == 13 && irInBuf[i].Event.KeyEvent.bKeyDown == false)
			{
				click.X = 0;
				click.Y = 21;
				break;
			}
		}
		if (click.Y % 2 == 1 && click.X % 2 == 1 && click.Y < 18 && click.X < 18) // If it's a valid move and it is not on a border
		{
			click.Y /= 2;
			click.X /= 2;
			return click;
		}
		else if (click.Y == 21) // Clicked to solve
			return click;
	}
}

int Sudoku::GetNumber()
{
	COORD pos = {0, 9*2+2};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(hOut, pos);
	cout << "Type a valid number from 1-9.                             \n";
	cout << "Space/Backspace/Del/C to clear. Esc to cancel.            ";

	while (true)
	{
		int key = getch(); // Wait for keyboard input

		if (key - 48 >= 1 && key - 48 <= 9) // Number 1-9
			return key - 48;
		else if (key == 32 || key == 8 || key == 83 || key == 99 || key == 67) // Space, Backspace, Delete, c, C
			return 0;
		else if (key == 27) // Esc
			return 10;
	}
}

int Sudoku::CountPossibleDigits(Coord position)
{
	int countPossible = 0;
	bool possible[10];
	for (int count = 0; count <=9; count ++)
		possible[count] = true;

	for (int row = 0; row < 9; row ++) // Evaluate current column
		possible[grid[row][position.X]] = false;
	for (int col = 0; col < 9; col ++) // Evaluate current row
		possible[grid[position.Y][col]] = false;

	// Evaluate current 3x3 box
	for (int row = (position.Y / 3) * 3; row < (position.Y / 3 + 1) * 3; row ++)
		for (int col = (position.X / 3) * 3; col < (position.X / 3 + 1) * 3; col ++)
			possible[grid[row][col]] = false;
	
	for (int count = 1; count <=9; count ++)
		if (possible[count])
			countPossible ++;

	return countPossible;
}

int Sudoku::GetDigit(Coord position)
{
	bool possible[10];
	for (int count = 0; count <=9; count ++)
		possible[count] = true;

	for (int row = 0; row < 9; row ++) // Evaluate current column
		possible[grid[row][position.X]] = false;
	for (int col = 0; col < 9; col ++) // Evaluate current row
		possible[grid[position.Y][col]] = false;

	// Evaluate current 3x3 box
	for (int row = (position.Y / 3) * 3; row < (position.Y / 3 + 1) * 3; row ++)
		for (int col = (position.X / 3) * 3; col < (position.X / 3 + 1) * 3; col ++)
			possible[grid[row][col]] = false;

	for (int count = 1; count <=9; count ++)
		if (possible[count])
			return count;
	return 0; // If nothing is possible, return 0
}

bool Sudoku::IsValid(COORD position, int digit)
{
	if (digit == 0) // We can clear anywhere
		return true;
	for (int row = 0; row < 9; row ++) // Search current column
		if (grid[row][position.X] == digit && position.Y != row)
			return false;
	for (int col = 0; col < 9; col ++) // Search current row
		if (grid[position.Y][col] == digit && position.X != col)
			return false;

	// Search current 3x3 box
	for (int row = (position.Y / 3) * 3; row < (position.Y / 3 + 1) * 3; row ++)
		for (int col = (position.X / 3) * 3; col < (position.X / 3 + 1) * 3; col ++)
			if (grid[row][col] == digit && (position.Y != row || position.X != col))
				return false;
	
	return true; // The digit was not found in any vertical line, horizontal line or box, so it is valid
}

void Sudoku::GetNumbers()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	error = false;
	Display();
	while (true)
	{
		COORD click = {0, 9*2+2};
		SetConsoleCursorPosition(hOut, click);
		cout << "Click any cell to change the number in the grid.            \n";
		SetConsoleTextAttribute(hOut, 0xF9); // Link
		cout << "Press Enter or click here to solve the sudoku.              ";
		SetConsoleTextAttribute(hOut, 0xF0); // Reset normal color

		COORD cell = GetClick();

		if (cell.Y == 21)
			break;
		else
		{
			click.Y = cell.Y * 2 + 1;
			click.X = cell.X * 2 + 1;
			SetConsoleCursorPosition(hOut, click);
			SetConsoleTextAttribute(hOut, HIGHLIGHT_COLOR); // Highlight
			if (grid[cell.Y][cell.X] == 0)
				cout << " ";
			else
				cout << grid[cell.Y][cell.X];
			SetConsoleTextAttribute(hOut, 0xF0); // Reset normal color

			int num = GetNumber();
			if (num != 10 && IsValid(cell, num))
				grid[cell.Y][cell.X] = num;
			click.Y = cell.Y * 2 + 1;
			click.X = cell.X * 2 + 1;
			SetConsoleCursorPosition(hOut, click);
			if (grid[cell.Y][cell.X] == 0)
				cout << " ";
			else
				cout << grid[cell.Y][cell.X];
		}
	}
}

void Sudoku::Solve()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	time_t timeSpent = time (NULL); // Initialize to current time

	for (int row = 0; row < 9; row ++)
		for (int col = 0; col < 9; col ++)
			empty[row][col] = (grid[row][col] == 0);

	COORD pos = {0, -1};
	MoveForward(pos);

	while (pos.Y < 9) // Until we hit a 10th row
	{
		grid[pos.Y][pos.X] ++;

		//Display(); // For Debug (shows step-by-step resolution)
		//system("pause");

		while (IsValid(pos, grid[pos.Y][pos.X]) == false)
		{
			//Display(); // For Debug (shows step-by-step resolution)
			//system("pause");

			grid[pos.Y][pos.X] ++;
			if (grid[pos.Y][pos.X] > 9)
				break;
		}
		if (grid[pos.Y][pos.X] > 9)
			MoveBackward(pos);
		else
			MoveForward(pos);

		if (difftime(time(NULL), timeSpent) > ERROR_TIME) // If the solution has taken more than ERROR_TIME seconds
		{
			error = true;
			break;
		}
	}
	if (error)
	{
		for (int row = 0; row < 9; row ++)
			for (int col = 0; col < 9; col ++)
				if (empty[row][col])
					grid[row][col] = 0;
		system("cls");
		cout << "This grid is not solvable.\n";
		cout << endl;
		cout << "Press any key to try again . . .";
		system("Pause > nul");
		/*Display();
		cout << "\nPress any key to quit . . .";
		system("Pause > nul");*/
	}
	else
	{
		SetConsoleTextAttribute(hOut, REVEAL_COLOR); // Change color of the reveal symbol
		for (int row = 0; row < 9; row ++)
		{
			for (int col = 0; col < 9; col ++)
			{
				if (empty[row][col])
				{
					COORD position = {col * 2 + 1, row * 2 + 1};
					SetConsoleCursorPosition(hOut, position);
					cout << REVEAL_CHAR;
				}
			}
		}
		SetConsoleTextAttribute(hOut, 0xF0); // Reset normal color
	}
}

void Sudoku::MoveForward(COORD& pos)
{	
	do
	{
		if (pos.X < 8)
			pos.X ++;
		else
		{
			pos.Y ++;
			pos.X = 0;
		}
	} while (empty[pos.Y][pos.X] == false && pos.Y < 9);

	if (pos.Y < 9)
		grid[pos.Y][pos.X] = 0;
}

void Sudoku::MoveBackward(COORD& pos)
{
	do
	{
		if (pos.X > 0)
			pos.X --;
		else
		{
			pos.Y --;
			pos.X = 8;
		}
	} while (empty[pos.Y][pos.X] == false && pos.Y >= 0);
}

void Sudoku::ShowSolution()
{
	if (!error)
	{
		COORD pos = {0, 9*2+2};
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleCursorPosition(hOut, pos);
		cout << "Click a ";
		SetConsoleTextAttribute(hOut, REVEAL_COLOR); // Change color of the reveal symbol
		cout << REVEAL_CHAR;
		SetConsoleTextAttribute(hOut, 0xF0); // Reset normal color
		cout << " to reveal the digit.                  \n";
		SetConsoleTextAttribute(hOut, 0xF9); // Link
		cout << "Press Enter or click here to show all.            ";

		SetConsoleTextAttribute(hOut, REVEALED_COLOR); // Change color of the revealed digits
		while (true)
		{
			COORD cell = GetClick();
			if (cell.Y == 21)
				break;
			else
			{
				if (empty[cell.Y][cell.X])
				{
					pos.Y = cell.Y * 2 + 1;
					pos.X = cell.X * 2 + 1;
					SetConsoleCursorPosition(hOut, pos);
					cout << grid[cell.Y][cell.X];
				}
			}
		}
		SetConsoleTextAttribute(hOut, 0xF0); // Reset color
		Display();
		cout << "\nPress any key to quit . . .";
		system("Pause > nul");
	}
}