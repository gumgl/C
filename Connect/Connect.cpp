#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

struct Intersection
{
	char C;//ASCII Character
	bool T, R, B, L;//Top, Right, Bottom, Left
};

void GetDimensions();
void Initialize();
void ResetArray(Intersection[]);
int Filter(char, bool);
void DrawGrid();
void CopyArray(Intersection[], Intersection[], int);
Intersection PickRandom();
void SwitchColors();

const int NUMBER_OF_CHARACTERS = 12;
const int SWITCH_INTERVAL = 4;

Intersection available[NUMBER_OF_CHARACTERS];
Intersection **grid = NULL;

int numberOfAvailable, ROWS, COLS, style, color;

void main(void)
{
	srand( time(NULL) );
	color = 0;
	SwitchColors();
	GetDimensions();
	grid = new Intersection *[ROWS];
	for( int i = 0 ; i < ROWS ; i++ )
		grid[i] = new Intersection[COLS];

	while (true)
	{
		system("cls");
		Initialize();

		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				if (row > 0)// First Row
					numberOfAvailable = Filter('T', grid[row-1][col].B);
				else
					numberOfAvailable = Filter('T', false);
				if (col > 0)// First Column
					numberOfAvailable = Filter('L', grid[row][col-1].R);
				else
					numberOfAvailable = Filter('L', false);

				if (row == ROWS - 1)// Last Row
				{
					numberOfAvailable = Filter('B', false);
					if (col == COLS - 2)
						numberOfAvailable = Filter('R', true);
				}
				if (col == COLS - 1)// Last Column
				{
					numberOfAvailable = Filter('R', false);
					if (row == ROWS - 2)
						numberOfAvailable = Filter('B', true);
				}
				if (row == ROWS - 2 && col == COLS - 2)// bottom-right corner must be connected to top & left
				{
					numberOfAvailable = Filter('B', true);
					numberOfAvailable = Filter('R', true);
				}
				grid[row][col] = PickRandom();
				cout << grid[row][col].C;
				ResetArray(available);
				numberOfAvailable = NUMBER_OF_CHARACTERS;
			}
			cout << endl;
		}

		cout << endl << endl;
		system("pause");
		SwitchColors();
	}
}

void GetDimensions()
{
	cout << "Width: ";
	cin  >> COLS;
	cout << "Height: ";
	cin  >> ROWS;
	cout << "Style (1 or 2): ";
	cin  >> style;
}

void Initialize()
{
	ResetArray(available);
	numberOfAvailable = NUMBER_OF_CHARACTERS;
}

void ResetArray(Intersection Array[])
{
	Intersection Default[] = {{' ', false, false, false, false},// (SPACE)
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
	//alternate style characters:
	Intersection Default2[] = {{' ', false, false, false, false},// (SPACE)
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
	
	if (style == 1)
		CopyArray(Default, Array, NUMBER_OF_CHARACTERS);
	else
		CopyArray(Default2, Array, NUMBER_OF_CHARACTERS);
}

int Filter(char direction, bool connecting)
{
	int count;
	int newPos = 0;
	Intersection newAvailable[NUMBER_OF_CHARACTERS];
	if (direction == 'T')
	{
		for (count = 0; count < numberOfAvailable; count ++)
			if (available[count].T == connecting)
			{
				newAvailable[newPos] = available[count];
				newPos ++;
			}
	}
	else if (direction == 'R')
	{
		for (count = 0; count < numberOfAvailable; count ++)
			if (available[count].R == connecting)
			{
				newAvailable[newPos] = available[count];
				newPos ++;
			}
	} 
	else if (direction == 'B')
	{
		for (count = 0; count < numberOfAvailable; count ++)
			if (available[count].B == connecting)
			{
				newAvailable[newPos] = available[count];
				newPos ++;
			}
	}
	else //direction == 'L'
	{
		for (count = 0; count < numberOfAvailable; count ++)
			if (available[count].L == connecting)
			{
				newAvailable[newPos] = available[count];
				newPos ++;
			}
	}
	CopyArray(newAvailable, available, newPos);
	return newPos;
}

void CopyArray(Intersection from[], Intersection to[], int size)
{
	for (int count = 0; count < size; count++)
		to[count] = from[count];
}

Intersection PickRandom() {
	int randomIndex = rand() % numberOfAvailable;
	return available[randomIndex];
}

void SwitchColors()
{
	if (color >= SWITCH_INTERVAL * 2)
		color = 0;
	if (color >= SWITCH_INTERVAL)
		system("color f0");
	else
		system("color 07");
	color++;
}