#include "othello.h"

using namespace std;

Othello::Othello()
{
	
}

void Othello::Play()
{
	Init();

	while (true)
	{
		while (Draw()) // Normal gameplay, gets out if no valid move
		{
			if (gameType > 0 && currentPlayer == Black)
				MakeAIMove();
			else
				GetMove();
		}
		SwitchPlayer();
		if (!Draw()) // If there is still no more valid move after switching player
			break; // End game
	}
	EndOfGame();
}

void Othello::Init()
{
	SetConsoleTitleW(_T("Othello (Reversi) by Guillaume L.")); // Change Console Title
	system("color 2F"); // Change Console Colors to White on Green
	
	gameType = 0;

	for (int row = 0; row < BOARD_SIZE; row ++) // Reset Rows
		for (int col = 0; col < BOARD_SIZE; col ++) // Reset Columns
			theBoard[row][col] = Blank;
	// Set starting pieces:
	theBoard[BOARD_SIZE / 2 - 1 ][BOARD_SIZE / 2 - 1] = Black;
	theBoard[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2] = White;
	theBoard[BOARD_SIZE / 2][BOARD_SIZE / 2 - 1] = White;
	theBoard[BOARD_SIZE / 2][BOARD_SIZE / 2] = Black;
	currentPlayer = START_PLAYER;

	cout << "Choose Game Type\n";
	cout << endl;
	cout << "1 - Human vs. Human\n";
	cout << "2 - Human vs. Computer (Easy)\n";
	cout << "3 - Human vs. Computer (Medium)\n";
	cout << "4 - Human vs. Computer (Hard)\n";
	cout << "5 - Human vs. Computer (In Development)\n";
	cout << endl;
	do
	{
		cout << "Game Type: ";
		cin  >> gameType;
	} while (gameType < 1 || gameType > 5);
	gameType --; // What the user entered is actually 1 more than what is stored in memory
}

bool Othello::Draw()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	int countWhite = 0;
	int countBlack = 0;
	int countMoves = 0;
	system("cls");
	SetConsoleTextAttribute(h, 0x20); // Set color to black on green (for borders)
	for (int row = 0; row <= BOARD_SIZE * 2; row ++) // Draw Rows
	{
		if (row % 2 == 0) // If row is even, display border row
		{
			switch (row)
			{
				case 0: // Top Row
					cout << char(218); // ┌
					break;
				case BOARD_SIZE * 2: // Bottom Row
					cout << char(192); // └
					break;
				default: // Middle Row
					cout << char(195); // ├
					break;

			}
			for (int col = 0; col < BOARD_SIZE - 1; col ++)
			{
				cout << char(196); // ─
				switch (row)
				{
					case 0: // Top Row
						cout << char(194); // ┬
						break;
					case BOARD_SIZE * 2: // Bottom Row
						cout << char(193); // ┴
						break;
					default: // Middle Row
						cout << char(197); // ┼
						break;
				}
			}
			cout << char(196); // ─
			switch (row)
			{
				case 0: // Top Row
					cout << char(191); // ┐
					break;
				case BOARD_SIZE * 2: // Bottom Row
					cout << char(217); // ┘
					break;
				default: // Middle Row
					cout << char(180); // ┤
					break;
			}
		}
		else // row is odd, display content row
		{
			for (int col = 0; col < BOARD_SIZE; col ++)
			{
				cout << char(179); // │
				switch (theBoard[row / 2][col])
				{
					case Blank:
						if (ValidMove(row / 2, col)) // If there is a valid move here
						{
							if (HIGHLIGHT_VALID_MOVES && !(gameType > 0 && currentPlayer == Black)) // Do not highlight valid moves when AI is playing
							{
								SetConsoleTextAttribute(h, 0xA0); // Highlight this spot
							}
							countMoves ++;
						}
						cout << ' ';
						break;
					case White:
						countWhite ++;
						SetConsoleTextAttribute(h, 0x2F);
						cout << PLAYER_TOKEN;
						break;
					case Black:
						countBlack ++;
						cout << PLAYER_TOKEN;
						break;
				}
				SetConsoleTextAttribute(h, 0x20); // Reset color to black on green
			}
			cout << char(179);
		}
		cout << endl;
	}
	SetConsoleTextAttribute(h, 0x2F); // Reset color to normal (white on green)
	if (countMoves >= 1)
	{
		if (gameType > 0 && currentPlayer == Black) // If AI is playing
		{
			if (INSTANT_AI)
				cout << "Computer is making a move . . .";
			else
			{
				cout << "Computer's turn.\n";
				cout << "Press any key to play . . . ";
				system("Pause > nul");
			}
		}
		else
		{
			cout << "Current Player: " << (currentPlayer == White ? "White" : "Black") << endl;
			if (HIGHLIGHT_VALID_MOVES)
			{
				cout << "Possible moves: " << countMoves << " (";
				SetConsoleTextAttribute(h, 0x2A);
				cout << "highlighted";
				SetConsoleTextAttribute(h, 0x2F);
				cout << ")" << endl;
			}
			cout << "White tokens: " << countWhite << endl;
			cout << "Black tokens: " << countBlack << endl;
			cout << "Click a cell to play . . . ";
		}
	}
	return countMoves >= 1; // Return true if there is at least 1 valid move
}

bool Othello::ValidMove(int row, int col)
{ // For current player, we need a number of opposite tokens, then the current player token
	int newRow;
	int newCol;

	if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) // Can't play outside the board!
		return false;
	if (theBoard[row][col] != Blank) // Can't play where there is already a token!
		return false;

	for (int offsetY = -1; offsetY <= 1; offsetY ++)
	{
		for (int offsetX = -1; offsetX <= 1; offsetX ++)
		{
			newRow = row + offsetY; // Set the new row & col according to the absolute coordinates and the relative offsets
			newCol = col + offsetX;
			if ((offsetY != 0 || offsetX != 0 ) && newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
			{	// If we are not on the current spot (we moved in a direction) and we are in the boundaries of the board
				while (theBoard[newRow][newCol] == (currentPlayer ^ Black ^ White)) // While the token is the opposite, keep searching in that direction
				{ // Now we need to find the player token again, going in the same direction (using offset variables)
					newRow += offsetY; // Move in the same direction
					newCol += offsetX;
					if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE)
						break; // If we are out of the board, stop searching in that direction
					if (theBoard[newRow][newCol] == Blank)
						break; // If we find a blank spot, stop searching in that direction
					if (theBoard[newRow][newCol] == currentPlayer)
						return true; // If we find the player token, SUCCESS! POSSIBLE MOVE!
				}
			}
		}
	}
	return false;
}

int Othello::MoveValue(int row, int col, int depth) // depth = # of moves to evaluate (depth = 2 means it will calculate the gain of the next 2 moves in the worst scenario for you)
{ // For current player, we need a number of opposite tokens, then the current player token
	int newRow;
	int newCol;
	int win = 0; // How many token we will win
	boardPiece playerBackup = currentPlayer;

	if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) // Can't play outside the board!
		return 0;
	if (theBoard[row][col] != Blank) // Can't play where there is already a token!
		return 0;
	if (depth < 1) // If we don't even want to know what this move will gain
		return 1;

	for (int offsetY = -1; offsetY <= 1; offsetY ++)
	{
		for (int offsetX = -1; offsetX <= 1; offsetX ++)
		{
			newRow = row + offsetY; // Set the new row & col according to the absolute coordinates and the relative offsets
			newCol = col + offsetX;
			if ((offsetY != 0 || offsetX != 0 ) && newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
			{	// If we are not on the current spot (we moved in a direction) and we are in the boundaries of the board
				while (theBoard[newRow][newCol] == (currentPlayer ^ Black ^ White)) // While the token is the opposite, keep searching in that direction
				{ // Now we need to find the player token again, going in the same direction (using offset variables)
					newRow += offsetY; // Move in the same direction
					newCol += offsetX;
					if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE)
						break; // If we are out of the board, stop searching in that direction
					if (theBoard[newRow][newCol] == Blank)
						break; // If we find a blank spot, stop searching in that direction
					if (theBoard[newRow][newCol] == currentPlayer)
					{// If we find the player token, we found a move! now going back to original position, counting each gain
						newRow -= offsetY; // Move in the opposite direction
						newCol -= offsetX;

						while (newRow != row || newCol != col)
						{ // While we are not on the original spot, we increment # of wins and move
							newRow -= offsetY; // Move in the opposite direction
							newCol -= offsetX;
							win ++;
						}
						break; // Move on to another direction
					}
				}
			}
		}
	}

	if (win > 0) // If we win something (move is valid)
	{
		win ++; // Add 1 to count the token that will be placed

		if ((row == 0 && col == 0) || // top-left corner
			(row == 0 && col == BOARD_SIZE - 1) || // top-right corner
			(row == BOARD_SIZE - 1 && col == 0) || // bottom-left corner
			(row == BOARD_SIZE - 1 && col == BOARD_SIZE - 1)) // bottom-right corner
			win += BOARD_SIZE * BOARD_SIZE;

		if (depth > 1) // We have to investigate further
		{
			//win += (depth * 2);
			//win = win * (depth / 5.0 + 1); // This gives more importance to moves closer to current game state, to prioritize more probable gameplays

			int gain;
			int maxGain = -4 * BOARD_SIZE * gameType; // Start with a really low value because after many moves, we could end up with maxGain being negative
			Othello* branch = new Othello(); // Create a new game
			*branch = *this; // With current state
			branch->MakeMove(row, col); // Make move that is being evaluated

			for (int switches = 0; switches <= 1; switches ++)
			{
				for (int r = 0; r < BOARD_SIZE; r ++) // Rows
				{
					for (int c = 0; c < BOARD_SIZE; c ++) // Columns
					{
						gain = branch->MoveValue(r, c, depth - 1);
						if (gain > maxGain)
							maxGain = gain;
					}
				}
				if (maxGain == 0)
				{
					branch->SwitchPlayer();
					SwitchPlayer();
				}
				else
					break;
			}
			if (currentPlayer == Black)
				win -= maxGain;
			else
				win += maxGain;

			delete [] branch; // very, VERY IMPORTANT to free memory. if it was not there, program would use 100 mb in a few moves!
		}
	}
	currentPlayer = playerBackup;

	return win;
}

void Othello::MakeMove(int row, int col)
	{ // For current player, we need a number of opposite tokens, then the current player token
	int newRow;
	int newCol;
	for (int offsetY = -1; offsetY <= 1; offsetY ++)
	{
		for (int offsetX = -1; offsetX <= 1; offsetX ++)
		{
			newRow = row + offsetY; // Set the new row & col according to the absolute coordinates and the relative offsets
			newCol = col + offsetX;
			if ((offsetY != 0 || offsetX != 0 ) && newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
			{	// If we are not on the current spot (we moved in a direction) and we are in the boundaries of the board
				while (theBoard[newRow][newCol] == (currentPlayer ^ Black ^ White)) // While the token is the opposite, keep searching in that direction
				{ // Now we need to find the player token again, going in the same direction (using offset variables)
					newRow += offsetY; // Move in the same direction
					newCol += offsetX;
					if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE)
						break; // If we are out of the board, stop searching in that direction
					if (theBoard[newRow][newCol] == Blank)
						break; // If we find a blank spot, stop searching in that direction
					if (theBoard[newRow][newCol] == currentPlayer)
					{// If we find the player token, we found a move! now going back to original position, flipping each token on our way
						while (newRow != row || newCol != col)
						{ // While we are not on the original spot, move back & flip that token
							newRow -= offsetY; // Move in the opposite direction
							newCol -= offsetX;
							theBoard[newRow][newCol] = currentPlayer;
						}
						break; // Move on to another direction
					}
				}
			}
		}
	}
	SwitchPlayer();
}

void Othello::MakeAIMove()
{
	int biggestGain = -4 * BOARD_SIZE * gameType; // Start with a really low value because after many moves, we could end up with biggestGain being negative
	int currGain;
	int depth;
	COORD move;
	Stack bestMoves;
	
	switch (gameType)
	{
	case 1:
		depth = 0; // Evaluate no move (play any valid move)
		break;
	case 2:
		depth = 1; // Evaluate wins by this move only
		break;
	case 3:
		depth = 2; // Evaluate wins by this move and the opponent's move
		break;
	case 4:
		depth = 5; // Evaluate wins by this move, the opponent's move and your next move
		break;
	}

	for (int row = 0; row < BOARD_SIZE; row ++) // Rows
	{
		for (int col = 0; col < BOARD_SIZE; col ++) // Columns
		{
			if (ValidMove(row, col)) // If move is valid
			{
				currGain = MoveValue(row, col, depth);
				move.Y = row;
				move.X = col;
				if (currGain > biggestGain) // If we found a new best move
				{
					biggestGain = currGain;
					bestMoves.Clear(); // Remove all the older moves, because this one is better
					bestMoves.Push(move); // Add this move to the stack
				}
				else if (currGain == biggestGain) // If this move is as good as the best one
					bestMoves.Push(move); // Add it to the stack
			}
		}
	}
	move = bestMoves.GetRandom();
	MakeMove(move.Y, move.X);
}

void Othello::GetMove()
{
	HANDLE hStdin;
    DWORD cNumRead, fdwMode, fdwSaveOldMode, i;
    INPUT_RECORD irInBuf[128];
    int counter=0;
	COORD mousePos;

    // Get the standard input handle.

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
				mousePos = irInBuf[i].Event.MouseEvent.dwMousePosition;
				break;
				//cout << CountGain(mousePos.Y / 2, mousePos.X / 2) << endl;
				/*cout << "Mouse Pos X: " << mousePos.X << endl;
				cout << "Mouse Pos Y: " << mousePos.Y << endl;*/
			}
		}
		if (ValidMove(mousePos.Y / 2, mousePos.X / 2) && mousePos.Y % 2 == 1 && mousePos.X % 2 == 1) // If it's a valid move and it is not on a border
		{
			MakeMove(mousePos.Y / 2, mousePos.X / 2);
			break;
		}
	}
}

void Othello::SwitchPlayer()
{
	currentPlayer = (boardPiece) (currentPlayer ^ White ^ Black);
}

void Othello::EndOfGame()
{
	int countWhite = 0;
	int countBlack = 0;

	for (int row = 0; row < BOARD_SIZE; row ++) // Rows
	{
		for (int col = 0; col < BOARD_SIZE; col ++) // Columns
		{
			if (theBoard[row][col] == White)
				countWhite ++;
			else if (theBoard[row][col] == Black)
				countBlack ++;
		}
	}
	cout << "End of game\n\n";
	if (gameType == 0)
		cout << (countWhite > countBlack ? "White wins!" : countBlack > countWhite ? "Black wins!" : "It's a tie!") << endl;
	else
		cout << (countWhite > countBlack ? "You win!" : countBlack > countWhite ? "Computer wins!" : "It's a tie!") << endl;
	cout << "White tokens: " << countWhite << endl;
	cout << "Black tokens: " << countBlack << endl;
	cout << "Press any key to quit . . . ";
	system("Pause > nul");
}