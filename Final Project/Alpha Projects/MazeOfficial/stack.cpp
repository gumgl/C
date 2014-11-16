#include "stack.h"
using namespace std;

int random( int min, int max)
{
	int range = max - min + 1;
	if (range == 0)
		return min;
	return rand() % range + min;
}

Stack::Stack()
{
	size = 10;
	current = -1;
	positions = new Coord[size];
}

Stack::Stack(int inSize)
{
	size = inSize;
	current = -1;
	positions = new Coord[size];
}

int Stack::Size()
{
	return current + 1;
}

void Stack::Clear()
{
	current = -1;
}

void Stack::Shuffle()
{
	for (int count = 0; count <= current; count ++)
		swap(positions[count], positions[random(count, current)]);
}

void Stack::Push(Coord inPoint)
{
	current ++; // Move the index upward
	if (current >= size) // If new size is bigger than current size of array
	{
		size *= 2; // Double size of the array
		Coord* temp = new Coord[size]; // Create new temporary array with bigger size
		for (int count = 0; count < current; count ++)
			temp[count] = positions[count]; // Put all the old Coordinates in the temporary array
		delete [] positions; // Free old array memory
		positions = temp; // Point to new array
	}
	positions[current] = inPoint; // Add inPoint to stack
}

Coord Stack::Pop()
{
	if (current < 0) // Stack is empty
	{
		Coord error; // Generates a (0,0) coordinate
		return error; // we *need* to return something even when stack is empty
	}
	else
	{
		Coord top = positions[current];
		current --; // move index downward
		return top;
	}
}

Coord Stack::Get(int index)
{
	return positions[index];
}

Coord Stack::GetMiddle()
{
	return positions[current / 2];
}

Coord Stack::GetRandom()
{
	return positions[random(0, current)];
}

Coord Stack::GetRandom(int start, int end)
{
	return positions[random(start, end)];
}

Coord::Coord()
{
	X = 0;
	Y = 0;
}

Coord::Coord(int y, int x)
{
	X = x;
	Y = y;
}

void Coord::Set(int y, int x)
{
	X = x;
	Y = y;
}

Coord Coord::operator+(Coord inCoord)
{
	Coord newCoord;
	newCoord.X = X + inCoord.X;
	newCoord.Y = Y + inCoord.Y;

	return newCoord;
}

bool Coord::operator==(Coord inCoord)
{
	return (X == inCoord.X && Y == inCoord.Y);
}
