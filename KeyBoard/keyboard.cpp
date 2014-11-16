#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

void main(void)
{
	const int SIZE = 10;
	const char SELECTED = 'O';
	const char EMPTY = '~';

	const int KEY_ENTER = 13;
	const int KEY_ESCAPE = 27;
	const int KEY_SPACE = 32;
	const int KEY_UP = 72;
	const int KEY_LEFT = 75;
	const int KEY_RIGHT = 77;
	const int KEY_DOWN = 80;
	
	int X = 0;
	int Y = 0;
	int key;

	while (true)
	{
		system("cls");
		for (int row = 0; row < SIZE; row ++)
		{
			for (int col = 0; col < SIZE; col ++)
				cout << ((row == Y && col == X) ? SELECTED : EMPTY);
			cout << endl;
		}

		key = getch();
		switch (key)
		{
			case KEY_LEFT:
				X = (X == 0) ? SIZE - 1 : X - 1;
				break;
			case KEY_RIGHT:
				X = (X == SIZE - 1) ? 0 : X + 1;
				break;
			case KEY_UP:
				Y = (Y == 0) ? SIZE - 1 : Y - 1;
				break;
			case KEY_DOWN:
				Y = (Y == SIZE - 1) ? 1 : Y + 1;
				break;
		}
	}
}