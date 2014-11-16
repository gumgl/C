#ifndef _DECK_
#define _DECK_

#include "card.h"

enum PokerRank {Nothing, Pair, TwoPair, ThreeOfAKind,
		Straight, Flush, FullHouse, FourOfAKind,
		StraightFlush, RoyalFlush};

class Deck
{
	private:
		Card theDeck[52];
		int size;
	public:
		// Build a proper deck with size=52
		Deck();
		// Build a proper deck but also specify size
		Deck(int);


		// Reset deck to a proper deck
		void Reset(int inSize = 52);
		
		// Should call Print for each Card left in the deck.
		void Print();

		// Draw the cards
		void PrintHand();

		// Must use time as the random generator seed.
		// Shuffle resets the deck to full
		void Shuffle();
  
		// Is the deck empty?
		bool IsEmpty();
  
		// Pre-condition: The deck is not empty.
		Card DealACard();

		// Return a DECK with the top X cards from this deck, removing them from this deck too
		Deck Deal(int);

		//Add a CARD to the top of the deck
		void Add(Card);

		//Add a DECK to the top of this deck
		void Add(Deck&);

		//Add a CARD to the bottom of the deck
		void AddBottom(Card);

		//Add a DECK to the bottom of the deck
		void AddBottom(Deck&);

		//Return number of cards in deck
		int Count();

		//Return the number of cards with a specific face value
		int Count(FaceValue);

		//Return the number of cards with a specific suit
		int Count(Suit);

		// Discard a card with specified index in the array.
		void Discard(int = 0);

		// Replace a specified card with a new card
		void Replace(int, Card);

		PokerRank Strength();
};

int random(int, int);

#endif