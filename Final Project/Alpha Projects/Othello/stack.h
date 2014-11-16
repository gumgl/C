#ifndef _STACK_
#define _STACK_

#include <iostream>
#include <string>
#include <ctime> // For random numbers
#include <Windows.h>

class Stack
{
	private:
		int current; // Current position of the top element. When there a no element, current = -1
		int size; // Current size of the array
		COORD * positions; // Pointer to an array of coordinates, can be expanded
	public:
		Stack();
		int Size(); // Returns the size of the stack, how many elements are in the stack (not of the size of the array)
		void Clear();
		void Push(COORD);
		COORD Pop();
		COORD GetMiddle(); // Returns the value of the middle element in the stack (to place Portal)
		COORD GetRandom(); // Returns a random element in the stack
};

int random(int, int);

#endif