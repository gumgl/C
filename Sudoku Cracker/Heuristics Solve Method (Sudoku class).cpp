void Sudoku::Solve()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, 0xFC); // Change color of the "?"
	while (true)
	{
		//Display(); // For Debug (shows step-by-step resolution)
		int filled = 0;
		int minPossible = 9;
		Stack cells(81);
		for (int row = 0; row < 9; row ++)
		{
			for (int col = 0; col < 9; col ++)
			{
				if (grid[row][col] == 0) // Cell is empty
				{
					Coord currentPos(row, col);
					int countPossible = CountPossibleDigits(currentPos);
					if (countPossible < minPossible)
					{
						minPossible = countPossible;
						cells.Clear();
						cells.Push(currentPos);
					}
					else if (countPossible == minPossible)
						cells.Push(currentPos);
				}
				else // Already a digit in that cell
					filled ++;
			}
		}
		if (filled == 9 * 9) // the whole grid is filled, the sudoku is solved
			break;
		if (minPossible == 1) // If we have cells that have only 1 possible digit
		{ // Change all these cells
			while (cells.Size() > 0)
			{
				Coord position = cells.Pop();
				grid[position.Y][position.X] = GetDigit(position);
				COORD pos = {position.X * 2 + 1, position.Y * 2 + 1};
				SetConsoleCursorPosition(hOut, pos);
				cout << "?";
			}
		}
		else // Only change 1 cell
		{
			int filled = 0;
			int minPossible = 9;
			Stack positions(9); // Possible positions for current number
			for (int digit = 1; digit <= 9; digit ++) // For each digit, we're looking in each row, col & box for 1 cell where it could go
			{
				for (int row = 0; row < 9; row ++) // For all rows
				{
					positions.Clear();
					for (int col = 0; col < 9; col ++) // For all cells in a row
					{
						if (grid[row][col] == 0) // Cell is empty
						{
							if (grid[row][col] == digit) // We found the digit in the current row
							{
								positions.Clear();
								break;
							}
							else
							{

							}
							Coord currentPos(row, col);
							int countPossible = CountPossibleDigits(currentPos);
							if (countPossible < minPossible)
							{
								minPossible = countPossible;
								cells.Clear();
								cells.Push(currentPos);
							}
							else if (countPossible == minPossible)
								cells.Push(currentPos);
						}
						else // Already a digit in that cell
							filled ++;
					}
				}
			}
			
			/*Coord position = cells.Get(0);
			grid[position.Y][position.X] = GetDigit(position);
			COORD pos = {position.X * 2 + 1, position.Y * 2 + 1};
			SetConsoleCursorPosition(hOut, pos);
			cout << "?";*/
		}
	}
	SetConsoleTextAttribute(hOut, 0xF0); // Reset normal color
}