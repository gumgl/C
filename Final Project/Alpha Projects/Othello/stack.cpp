#include "stack.h"

int random( int min, int max)
{
	int range = max - min + 1;
	if (range == 0)
		return min;
	return rand() % range + min;
}

Stack::Stack()
{
	srand ( (unsigned int)time(NULL) );
	size = 10;
	current = -1;
	positions = new COORD[size];
}

int Stack::Size()
{
	return current + 1;
}

void Stack::Clear()
{
	current = -1;
}

void Stack::Push(COORD inPoint)
{
	current ++;
	if (current >= size) // If new size is bigger than current size of array
	{
		size *= 2; // Double size of the array
		COORD* temp = new COORD[size]; // Create new temporary array with bigger size
		for (int count = 0; count < current; count ++)
			temp[count] = positions[count]; // Put all the old coordinates in the temporary array
		delete [] positions; // free old array memory
		positions = temp; // point to new array
	}
	positions[current] = inPoint;
}

COORD Stack::Pop()
{
	if (current < 0) // Stack is empty
	{
		COORD error = {0, 0};
		return error;
	}
	else
	{
		COORD top = positions[current];
		current --;
		return top;
	}
}

COORD Stack::GetMiddle()
{
	return positions[current / 2];
}

COORD Stack::GetRandom()
{
	return positions[random(0, current)];
}