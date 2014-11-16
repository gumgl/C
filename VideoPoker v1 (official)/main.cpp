#include <iostream>
#include <iomanip>
#include <string>
#include "deck.h"
#include "card.h"
using namespace std;

void main(void)
{
	const int MAX_DISCARD = 4;
	const int INIT_BANKROLL = 1000;

	int  bankroll = INIT_BANKROLL; // Player bank roll in $
	int  bet = 0;
	int  discarded = 0; // s# of cards discarded by player. max = 4
	int  input = 0; // User input
	int  rounds = 1;
	Deck deck; // The deck
	Deck hand(0); // Player's hand

	while (true)
	{
		discarded = 0;
		deck.Reset();
		deck.Shuffle(); // Get a new and shuffled deck every round

		system("cls");
		cout << "Round " << rounds << endl;
		cout << endl;

		do
		{
			cout << "How much would you like to bet (in $, 0 to quit)? ";
			cin  >> bet;
		} while (bet > bankroll);

		if (bet == 0)
		{
			cout << "Thank you for playing, you have " << ((INIT_BANKROLL - bankroll > 0) ? "lost" : "won") << " " << abs(INIT_BANKROLL - bankroll) << " dollars!\n";
			break;
		}

		bankroll -= bet; // Steal the money from their bank roll

		hand = deck.Deal(5); // Deal 5 cards to player

		while (discarded < MAX_DISCARD)
		{
			system("cls");

			cout << "Here are the cards in your hands: \n";
			hand.Print();
			cout << "You are allowed to discard " << MAX_DISCARD - discarded << " card" << ((MAX_DISCARD - discarded > 1) ? "s" : "") << ".\n";
			cout << "Type in the number of the card you wish to discard (0 to skip): ";
			cin  >> input;

			while (input < 0 || input > 5)
			{
				cout << "Please enter the number of the card or 0 to skip: ";
				cin >> input;
			}
			if (input == 0)
				break;
			else
			{
				hand.Replace(input - 1, deck.DealACard());
				//hand.Add(deck.DealACard());
				discarded ++;
			}
		}

		cout << "Evaluating your hand...\n";
		
		hand.Strength();
		
		
		
		system("pause");

		if (bankroll == 0) // They're broke
		{
			cout << "Thank you for playing Video Poker."; // Fake "Thank You" message
			break; // GTFO of the program, no more money to make...
		}
		rounds ++;
	}
	cout << endl;
	system("pause");
}