/*
	Guillaume Labranche
	Michael Grout

	Final Programming II Project
	
	Bonus features for the maze:
	- Cheating (no clip, break walls)
	- Trail (can be switched on/off in game)
	- Portal (redraws maze at portalPos)
	- Use of ASCII for the walls
	- Prim's Algorithm
	- Fast UI (no redrawing every time)
	- Customizable UI (colors and characters)

	And for Othello:
	- Artificial Intelligence
	- Point and Click UI
	- Customizable UI (possible moves and instant AI)

	1,516 lines of code
*/

#include "Maze.h"
#include "othello.h"
using namespace std;

void main(void)
{
	int key;
	// MENU
	cout << "Welcome to our game!\n";
	cout << endl;
	cout << "Please press the number of the game/type you want to play\n";
	cout << endl;
	cout << "Othello:\n";
	cout << " 1 - Human vs. Human\n";
	cout << " 2 - Human vs. Computer (Easy)\n";
	cout << " 3 - Human vs. Computer (Medium)\n";
	cout << " 4 - Human vs. Computer (Hard)\n";
	cout << endl;
	cout << "Maze:\n";
	cout << " 5 - Portal (long)\n";
	cout << " 6 - Prim's Algorithm (no portal but difficult)\n";
	
	do // USER INPUT
	{
		key = getch();
		key -= 48;
	} while (key < 1 || key > 6);
	
	system("cls");

	// LOAD GAME
	switch (key)
	{
		case 1: // No AI
		{
			Othello aGame(0);
			aGame.Play();
			break;
		}
		case 2: // Play any possible move
		{
			Othello aGame(1);
			aGame.Play();
			break;
		}
		case 3: // Computer looks 1 move ahead
		{
			Othello aGame(2);
			aGame.Play();
			break;
		}
		case 4: // Computer looks 6 moves ahead
		{
			Othello aGame(3);
			aGame.Play();
			break;
		}
		case 5:
		{
			Maze aMaze(false);
			aMaze.Go();
			break;
		}
		case 6:
		{
			Maze aMaze(true);
			aMaze.Go();
			break;
		}
	}
}