#include "sudoku.h"
using namespace std;

void main (void)
{
	Sudoku sudoku;

	do
	{
		sudoku.GetNumbers();
		sudoku.Solve();
		sudoku.ShowSolution();
	} while (sudoku.error);
}