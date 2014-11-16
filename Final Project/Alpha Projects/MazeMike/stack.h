#ifndef _STACK_
#define _STACK_

#include <iostream>
#include <string>
#include <ctime> // For random numbers
#include <Windows.h>

class Coord
{
	public:
		int Y; // Rows
		int X; // Columns
		Coord();
		Coord(int, int);
		bool operator == (Coord);
		Coord operator + (Coord);
		void Set(int, int);
};

class Stack
{
	private:
		int current; // Current position of the top element. When there a no element, current = -1
		int size; // Current size of the array
		Coord * positions; // Pointer to an array of Coordinates, can be expanded
	public:
		Stack();
		Stack(int);
		int Size(); // Returns the size of the stack, how many elements are in the stack (not of the size of the array)
		void Clear(); // Empties the array
		void Shuffle(); // Rearranges all the elements in a random order
		void Push(Coord);
		Coord Pop();
		Coord Get(int); // Returns a specific element in the stack
		Coord GetMiddle(); // Returns the value of the middle element in the stack (to place Portal)
		Coord GetRandom(); // Returns a random element in the stack
		Coord GetRandom(int, int);
};

int random(int, int);

#endif