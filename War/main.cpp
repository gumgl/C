#include <iostream>
#include <iomanip>
#include <string>
#include "deck.h"
#include "card.h"
using namespace std;

void DispCardsDist(int, int);
double p(int);

void main( void )
{
	/*number of cards dealt face-down when there is a tie
	Ex:	round1 - players deal 1 card each and it's a tie
		we add WAR_SIZE cards face-down
		round2 - players deal 1 card each and winners wins 2 + 2 + (WAR_SIZE * 2) cards */
	const int WAR_SIZE = 3;
	
	Deck deck1; //standard deck
	Deck deck2(10); //empty deck
	Deck stack(0); //extra deck to handle ties
	Card card1, card2; //player cards
	int points1 = 0;
	int points2 = 0;
	int round = 1;
	int wars = 0;
	int strength1, strength2;
	double p1, p2;

	deck1.Shuffle();
	deck2 = deck1.Deal(26); //split deck1 in two and give one half to deck2
	strength1 = deck1.Strength();
	strength2 = deck2.Strength();
	p1 = p(strength1);
	p2 = p(strength2);

	cout << "The game is about to begin! But before, some predictions:\n\n";
	cout << "Player One Hand Strength: " << strength1 << " (" << deck1.Count(Ace) << " aces)" << endl;
	cout << "Player Two Hand Strength: " << strength2 << " (" << deck2.Count(Ace) << " aces)" << endl;
	cout << endl;

	if (p2 < 0.5)
		cout << "Player One has " << fixed << setprecision(2) << p1 * 100 << "% chance of winning!\n";
	else if (p1 < 0.5)
		cout << "Player Two has " << fixed << setprecision(2) << p2 * 100 << "% chance of winning!\n";
	else
		cout << "Both players have 50% chance of winning.\n";

	cout << endl;
	system("pause");

	while (!deck1.IsEmpty() && !deck2.IsEmpty())
	{
		cout << "Round " << round << endl;

		card1 = deck1.DealACard();
		card2 = deck2.DealACard();
		stack.Add(card1);
		stack.Add(card2);
		stack.Shuffle();

		cout << "Player One Card = " << card1 << endl;
		cout << "Player Two Card = " << card2 << endl;

		if (card1 > card2)
		{
			int a;
			cout << "Player One wins round, they get " << stack.Count() << " cards!\n";
			deck1.AddDeck(stack);
		}
		else if (card1 < card2) {
			cout << "Player Two wins round, they get " << stack.Count() << " cards!\n";
			deck2.AddDeck(stack);
		}
		else // card1 == card2
		{
			wars ++;
			for (int count = 1; count <= WAR_SIZE; count ++)// add WAR_SIZE more cards to stack from each player deck
			{
				if (!deck1.IsEmpty())
					stack.Add(deck1.DealACard());
				if (!deck2.IsEmpty())
					stack.Add(deck2.DealACard());
			}
			cout << "War!!! There are " << stack.Count() << " cards on the table!\n";
		}
		
		round += 1;

		cout << "Current Score:\n";
		cout << "Player One - " << deck1.Count() << " cards" << endl;
		cout << "Player Two - " << deck2.Count() << " cards" << endl;
		DispCardsDist(deck1.Count(), deck2.Count());
		//system("pause");
		cout << endl;
	}

	cout << "Game Over!\n";
	
	if ( !stack.IsEmpty() ) //there are cards left in the stack (means last round was a war)
	{
		cout << "Last round was a war, so the winner of the game got " << stack.Count() << " cards!\n";
		//winner gets the cards left:
		if ( !deck1.IsEmpty() ) //player 1 won
			deck1.AddDeck(stack);
		else //player 2 won
			deck2.AddDeck(stack);
	}

	cout << "Initial Hand Strengths:\n";
	cout << "Player One = " << strength1 << " points" << endl;
	cout << "Player Two = " << strength2 << " points" << endl;
	cout << "Total Number of Wars: " << wars << endl;
	cout << "Final Score:\n";
	cout << "Player One - " << deck1.Count() << " cards" << endl;
	cout << "Player Two - " << deck2.Count() << " cards" << endl;

	if ( deck2.IsEmpty() )
		cout << "Player One Wins";
	else
		cout << "Player Two Wins";

	cout << endl;
	system("pause");
}

void DispCardsDist(int count1, int count2)
{
	int i;

	for (i = 1; i <= count1; i ++)
		cout << "+";
	cout << " ";
	for (i = 1; i <= count2; i ++)
		cout << "+";
}

double p(int S)
{
	return 0.0075 * S + 0.5021;
}